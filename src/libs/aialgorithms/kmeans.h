//
// Created by Dean Calver on 25/03/2016.
//

#pragma once

#include "aicore/featurevector.h"
#include <boost/dynamic_bitset.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/range/irange.hpp>

namespace AIAlgorithms {

    /*
     * KMeans sements the data into clusters, K clusters are output
     * each cluster has a centroid and a list of observation that are in it
     */
    template<typename REAL, typename ALU>
    class Cluster {
    public:
        typedef size_t ObservationIndex;

        Cluster(const size_t _dims) :
                dimensions(_dims) {
            centroid.resize(dimensions);
        }

        const size_t dimensions;
        typename ALU::template VectorOf<REAL> centroid;
        std::set<ObservationIndex> observers;
    };

    /*
     * KMeans in a unsupervised classifier. Generate K class from N dimensional data
     */
    template<typename REAL, typename ALU = Core::VectorALU<Core::VectorALUBackend::BASIC_CPP>>
    class KMeans {
    public:
        typedef REAL value_type;
        typedef ALU alu_type;

        typedef AICore::FeatureVector<value_type, alu_type> FVector;
        typedef AICore::FeatureSpace<value_type, alu_type> FSpace;

        KMeans(const size_t _k, const AICore::FeatureVectorBase::shared_ptr observations) :
                k(_k),
                dims(observations->dimensions),
                featureVector(std::static_pointer_cast<FVector>(observations)),
                featureSpace(dims, observations->count),
                featuresCurrentCluster(observations->count, ~1) {
            using namespace Core;
            using namespace AICore;
            using namespace boost;

            // we use Forgy Initialisation and the Core Random singleton
            // that should be seeded prior and should be a quick, good but not security
            // level randomiser (mersenne twister is a good start)
            clusters.reserve(k);

            typedef uniform_int<> distribution_type;
            typedef variate_generator<Random::generator_type &, distribution_type> gen_type;
            gen_type kRandGen(Random::generator, distribution_type(0, observations->count - 1));
            boost::generator_iterator<gen_type> kIter(&kRandGen);

            for (auto &&i : irange(0, (int) k)) {
                clusters.emplace_back(dims);

                auto &cluster = clusters.back();
                typename Cluster<REAL, ALU>::ObservationIndex obsI = -1;
                while (obsI == -1) {
                    obsI = *kIter++;
                    if (featuresCurrentCluster[obsI] != ~1) {
                        obsI = -1;
                    } else {
                        typename FSpace::tuple centroid = featureSpace.gather(featureVector->data(obsI));
                        cluster.centroid = centroid;
                        cluster.observers.insert(obsI);
                        featuresCurrentCluster[obsI] = i;
                    }
                }
            }
        }

        bool iterate() {
            bool done = assignment();
            updateCentroid();
            return done;
        }

        friend std::ostream &operator<<(std::ostream &out, const KMeans &a) {
            for (auto &&cluster : a.clusters) {
                for (auto &&observer : cluster.observers) {
                    auto val = a.featureSpace.gather(a.featureVector->data(observer));
                    for (auto &&item : val) {
                        out << item << ",";
                    }
                    out << "\"(";
                    bool ignoreComma = true;
                    for (auto &&v : cluster.centroid) {
                        if (!ignoreComma) {
                            out << ",";
                        } else {
                            ignoreComma = false;
                        }
                        out << v;
                    }
                    out << ")\"\n";
                }
            }
            return out;
        }

        const size_t k;
        const size_t dims;
    protected:
        bool assignment() {
            bool done = true;

            for (size_t j = 0; j < featureVector->count; ++j) {
                // find closest cluster to each feature
                REAL bestDist = std::numeric_limits<REAL>::infinity();
                int closestCluster = -1;
                int i = 0;
                for (auto &&cluster : clusters) {
                    REAL d = featureSpace.euclideanDistance(cluster.centroid, featureVector->data(j));
                    if (d < bestDist) {
                        bestDist = d;
                        closestCluster = i;
                    }
                    ++i;
                };
                // if feature has moved cluster update and continue iterating
                if (featuresCurrentCluster[j] != closestCluster) {
                    if (featuresCurrentCluster[j] != ~1) {
                        size_t mustBe1 = clusters[featuresCurrentCluster[j]].observers.erase(j);
                        assert(mustBe1 == 1);
                    }
                    featuresCurrentCluster[j] = (size_t) closestCluster;
                    clusters[closestCluster].observers.insert(j);
                    done = false;
                }
            }
            return done;
        }

        void updateCentroid() {
            for (auto &&cluster : clusters) {
                const typename FSpace::tuple divider(featureSpace.dimensions, cluster.observers.size());

                featureSpace.alu().set(REAL(0), cluster.centroid);
                for (auto &&observer : cluster.observers) {
                    auto val = featureSpace.gather(featureVector->data(observer));
                    featureSpace.alu().add(val, cluster.centroid, cluster.centroid);
                }
                featureSpace.alu().div(cluster.centroid, divider, cluster.centroid);
            }
        }

        typename FVector::shared_ptr featureVector;
        FSpace featureSpace;

        std::vector<size_t> featuresCurrentCluster;

        std::vector<Cluster<REAL, ALU>> clusters;
    };

}



//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include <ostream>
#include <map>
#include "User.h"
#include <algorithm>
#include <cmath>
#include <numeric>


struct compare_ranks {
    bool operator()(const std::pair<sp_movie, double>& lhs,
        const std::pair<sp_movie, double>& rhs) const {
      return lhs.second > rhs.second;
    }
};

struct compare_movie {
    bool operator()(const sp_movie& lhs, const sp_movie& rhs) const {
      return *lhs < *rhs;
    }
};


typedef std::map<sp_movie,const std::vector<double>,compare_movie> MovieMap;

class RecommendationSystem{

  MovieMap movie_map;

  double get_average(const User& user);
  rank_map normalize(const User& user);
  std::vector<double> preference_vector(const User& user);
  double vec_len(std::vector<double> vec);
  double similarity(const std::vector<double>& pref_vec, const
              std::vector<double>& features);
//  double rank_prediction(const User &user, const sp_movie &movie,int k,
//                        const std::map<sp_movie, double>& sim_set);

  public:

	explicit RecommendationSystem();


    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on
     * movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest predicted
     * score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using
     * item cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


  friend std::ostream &
  operator<< (std::ostream &os, const RecommendationSystem &rs);


};


#endif //RECOMMENDATIONSYSTEM_H

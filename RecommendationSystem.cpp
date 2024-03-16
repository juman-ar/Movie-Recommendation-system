//
// Created by hp on 3/8/2024.
//

#include "RecommendationSystem.h"


RecommendationSystem::RecommendationSystem()= default;

sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
                                         const std::vector<double>& features){
  Movie movie(name, year);
  sp_movie new_movie= std::make_shared<Movie>(movie);
  movie_map.insert({new_movie, features});
  return new_movie;
}

sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const{
  sp_movie new_movie = std::make_shared<Movie>(name, year);
  auto found= movie_map.find (new_movie);
  if( found == movie_map.end()){
    return nullptr;
  }
  return found->first;
}


////recommendation by content////

double RecommendationSystem::get_average(const User& user){
  int elem_num=0;
  double rank_sum=0;
  for(const auto &pair : user.get_ranks()){
//    if(pair.second !=0){
      elem_num++;
      rank_sum+= pair.second;
//    }
  }
  if(!elem_num){
    return 0;
  }
  return rank_sum/elem_num;
}

rank_map RecommendationSystem::normalize(const User& user){
  rank_map ranks = user.get_ranks();
  double avg= get_average (user);
  for(auto& pair : ranks){
//    if(pair.second != 0){
      pair.second -= avg;
//    }
  }
  return ranks;
}

std::vector<double> RecommendationSystem::preference_vector(const User& user){
  if(movie_map.empty()){
    return std::vector<double>();
  }
  std::vector<double> result(movie_map.begin()->second.size(), 0.0);
  rank_map ranks = normalize (user);


  for(const auto& pair : ranks){
    auto movie_it = movie_map.find (pair.first);
    if(movie_it != movie_map.end()){
      for(size_t i=0; i<result.size();i++){
        result[i] += pair.second * movie_it->second[i];
      }

//      std::vector<double> features= movie_map.find (pair.first)->second;
//      for(size_t i=0; i< result.size(); i++){
//        result[i] += (pair.second* features[i]);
//     }

    }
  }
  return result ;
}



double RecommendationSystem::vec_len(const std::vector<double>& vec){
  double sqr_sum=0;
  for(double value: vec){
    sqr_sum+= (value*value);
  }
  return sqrt(sqr_sum);
}

double RecommendationSystem::similarity(const std::vector<double>& pref_vec,
              const std::vector<double>& features){

//  double vec_product= std::inner_product (pref_vec.begin(), pref_vec.end(),
//                                          features.begin(), 0);
 double vec_product= 0;
 for(int i=0;i< (int)pref_vec.size();i++){
   vec_product+= pref_vec[i]*features[i];
 }
  double vec_lengths = vec_len (pref_vec)* vec_len(features);

  if(vec_lengths!=0){
    return vec_product/vec_lengths;
  }
  return 0;
}


sp_movie RecommendationSystem::recommend_by_content(const User& user){
  std::vector<double> users_pref= preference_vector (user);
  if(users_pref.empty()) {
    return nullptr;
  }
//  rank_map n_ranks= normalize (user);
  sp_movie rec_movie = nullptr;
  double max_similarity=-1 ;

  for(const auto& pair : movie_map){
//    if(n_ranks.find (pair.first) != n_ranks.end()){
//      continue;
//    }
    double sim = similarity (users_pref, pair.second);
    if(sim > max_similarity){
      max_similarity= sim;
      rec_movie= pair.first;
    }
  }
  return rec_movie;
}





////Collaborative filtering////


double RecommendationSystem::predict_movie_score(const User &user,
                                   const sp_movie &movie, int k) {
  rank_map users_rank = user.get_ranks();
  std::vector<std::pair<sp_movie, double>> similarity_scores;

  // Calculate similarity scores for each movie the user has rated
  for (const auto &user_movie_pair : users_rank) {
    if (movie_map.find(user_movie_pair.first) == movie_map.end()){
      continue;
    } // Skip if movie not found

    double sim = similarity(movie_map.at(user_movie_pair.first),
                            movie_map.at(movie));
    if (sim > 0) { // Consider only positive similarities
      similarity_scores.push_back(std::make_pair(user_movie_pair.first, sim));
    }
  }

  // Sort by similarity score in descending order
  std::sort(similarity_scores.begin(),
            similarity_scores.end(), compare_ranks());

  // Use top-k similar movies for prediction
  double weighted_sum = 0, sim_sum = 0;
  for (int i = 0; i < k && i < (int)similarity_scores.size(); ++i) {
    double sim = similarity_scores[i].second;
    weighted_sum += sim * users_rank.at(similarity_scores[i].first);
    sim_sum += sim;
  }

  return sim_sum > 0 ? weighted_sum / sim_sum : 0; // Avoid division by zero
}


sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k){
  rank_map users_rank= user.get_ranks();
  sp_movie rec_movie = nullptr;
  double max_prediction=0 ;

  for(const auto& pair : movie_map){
    if(users_rank.find (pair.first) != users_rank.end()){
      continue;
    }
    double prediction= predict_movie_score (user, pair.first,k);
    if(prediction> max_prediction){
      max_prediction= prediction;
      rec_movie= pair.first;
    }
  }

  return rec_movie;
}


std::ostream &operator<< (std::ostream &os, const RecommendationSystem& rs){
  for(const auto& pair : rs.movie_map){
    os<< *(pair.first);
  }
  return os;
}


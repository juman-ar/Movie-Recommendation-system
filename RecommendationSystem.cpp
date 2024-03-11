//
// Created by hp on 3/8/2024.
//

#include "RecommendationSystem.h"


RecommendationSystem::RecommendationSystem(){

}

sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
                                         const std::vector<double>& features){
  sp_movie new_movie = std::make_shared<Movie>(name, year);
  movie_map.insert({new_movie, features});
  return new_movie;
}

sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const{
  sp_movie new_movie = std::make_shared<Movie>(name, year);
  if(movie_map.find (new_movie) != movie_map.end()){
    return new_movie;
  }
  return nullptr;
}


////recommendation by content////

double RecommendationSystem::get_average(const User& user){
  int elem_num=0;
  double rank_sum=0;
  for(const auto &pair : user.get_ranks()){
    if(pair.second){
      elem_num++;
      rank_sum+= pair.second;
    }
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
    if(pair.second){
      pair.second -= avg;
    }
  }
  return ranks;
}

std::vector<double> RecommendationSystem::preference_vector(const User& user){
  std::vector<double> result;
  //start every element in the vector with 0
  for(auto i : movie_map.begin()->second){
    result.push_back(0);
  }
  rank_map ranks = normalize (user);

  for(const auto& pair : ranks){
    if(pair.second){
      std::vector<double> features= movie_map.find (pair.first)->second;
      for(int i=0; result.size(); i++){
        result[i] += (pair.second* features[i]);
      }
    }
  }
  return result ;
}



double RecommendationSystem::vec_len(std::vector<double> vec){
  double sqr_sum=0;
  for(double value: vec){
    sqr_sum+= (value*value);
  }
  return sqrt(sqr_sum);
}

double RecommendationSystem::similarity(const std::vector<double>& pref_vec,
              const std::vector<double>& features){

  double vec_product= std::inner_product (pref_vec.begin(), pref_vec.end(),
                                          features.begin(), 0);
  double vec_lengths = vec_len (pref_vec)* vec_len(features);
  if(vec_lengths){
    return vec_product/vec_lengths;
  }
  return 0;
}


sp_movie RecommendationSystem::recommend_by_content(const User& user){
  std::vector<double> users_pref= preference_vector (user);
  rank_map n_ranks= normalize (user);
  sp_movie rec_movie = nullptr;
  double max_similarity=0 ;
  for(const auto& pair : movie_map){
    if(n_ranks.find (pair.first) != n_ranks.end()){
      continue;
    }
    double sim = similarity (users_pref, pair.second);
    if(sim > max_similarity){
      max_similarity= sim;
      rec_movie= pair.first;
    }
  }
  return rec_movie;
}





////Collaborative filtering////

sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k){

}


double RecommendationSystem::predict_movie_score(const User &user, const
sp_movie &movie,
                           int k){

}





std::ostream &operator<< (std::ostream &os,
        const RecommendationSystem& rs){

  return os;
}


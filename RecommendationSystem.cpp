//
// Created by hp on 3/8/2024.
//

#include "RecommendationSystem.h"


RecommendationSystem::RecommendationSystem()= default;

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
  std::vector<double> result;
  //start every element in the vector with 0
  size_t vec_size=movie_map.begin()->second.size();
//  for(auto i : movie_map.begin()->second){
//    result.push_back(0);
//  }
  std::fill_n(result.begin(),vec_size, 0);

  rank_map ranks = normalize (user);

  for(const auto& pair : ranks){
//    if(pair.second!=0){
      std::vector<double> features= movie_map.find (pair.first)->second;
      for(size_t i=0; i< result.size(); i++){
        result[i] += (pair.second* features[i]);
//      }
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
  rank_map n_ranks= normalize (user);
  sp_movie rec_movie = nullptr;
  double max_similarity=0 ;
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

//double RecommendationSystem::rank_prediction(const User &user, const sp_movie
//&movie,int k, const std::map<sp_movie, double> &sim_set){
//
//  rank_map users_rank = user.get_ranks();
//  int count = 0;
//  double numerator = 0;
//  double denominator = 0;
//  for(const auto& pair : sim_set){
//    if(count < sim_set.size() - k){
//      count += 1;
//      continue;
//    }
//    denominator+= pair.second;
//    numerator += pair.second * users_rank.find(pair.first)->second ;
//  }
//
//  return numerator/denominator;
//
//}
//
//double RecommendationSystem::predict_movie_score(const User &user, const
//sp_movie &movie,int k){
//  rank_map users_rank = user.get_ranks();
//  std::vector<double> movie_features= movie_map.find(movie)->second;
//  std::map<sp_movie, double> similarity_set;
//  for(const auto& p : users_rank){
//    similarity_set.insert({p.first, similarity (movie_map.find (p.first)
//    ->second, movie_features)});
//  }
//  return rank_prediction (user, movie, k , similarity_set);
//}

double RecommendationSystem::predict_movie_score(const User &user, const
sp_movie &movie,int k){
  rank_map users_rank = user.get_ranks();
  std::vector<std::pair<sp_movie, double>> movie_sim;

  for (const auto& pair : movie_map) {
    if (users_rank.find(pair.first) == users_rank.end()){
      continue;
    }
    double sim_score = similarity(pair.second, movie_map.at(movie));
    movie_sim.emplace_back(pair.first,sim_score * users_rank.at(pair.first));
  }

  std::nth_element(movie_sim.begin(), movie_sim.begin() + k, movie_sim
      .end(), compare_ranks());

  double k_sum = std::accumulate(movie_sim.begin(), movie_sim.begin() + k,
        0,[](double acc, const auto& pair)
        {return acc + pair.second;});

  double similarity_sum = std::accumulate(movie_sim.begin(),
     movie_sim.begin() + k, 0,[](double acc, const auto& pair)
     {return acc + std::abs(pair.second);});

  if(similarity_sum==0){
    return 0;
  }

  return k_sum/similarity_sum;
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


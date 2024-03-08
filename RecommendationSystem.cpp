//
// Created by hp on 3/8/2024.
//

#include "RecommendationSystem.h"


RecommendationSystem::RecommendationSystem(){

}
sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
                                        const std::vector<double>& features){


}

sp_movie RecommendationSystem::recommend_by_content(const User& user){

}


sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k){

}


double RecommendationSystem::predict_movie_score(const User &user, const
sp_movie &movie,
                           int k){

}


sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const{

}


std::ostream &operator<< (std::ostream &os,
        const RecommendationSystem& rs){

  return os;
}


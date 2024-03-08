

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here

User::User (const std::string& name, const rank_map &ranks) : _username(name),
_ranks(ranks){}


const std::string &User::get_username () const
{
  return _username;
}


const rank_map &User::get_ranks () const
{
  return _ranks;
}


void User::add_movie_to_rs(const std::string &name, int year,
        const std::vector<double> &features, double rate){
  sp_movie new_movie(name, year);
  _ranks[new_movie] = rate;


}


sp_movie User::get_recommendation_by_content() const{

}


sp_movie User::get_recommendation_by_cf(int k) const{

}


double User::get_prediction_score_for_movie(const std::string& name, int year,
                         int k) const{

}


std::ostream &operator<< (std::ostream &os, const User &user)
{
  os << "_username: " << user._username;
  return os;
}


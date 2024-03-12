

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here

User::User (const std::string& name, const rank_map &ranks , const
std::shared_ptr<RecommendationSystem>& recommendation_system) :
_recommendation_system(recommendation_system),
_ranks(ranks){
  this->_username= name;
}


const std::string &User::get_name () const
{
  return _username;
}


const rank_map &User::get_ranks () const
{
  return _ranks;
}


void User::add_movie_to_rs(const std::string &name, int year,
        const std::vector<double> &features, double rate){
  sp_movie new_movie= _recommendation_system->add_movie (name,year, features);
  _ranks.insert ({new_movie, rate});

}


sp_movie User::get_recommendation_by_content() const{
  return _recommendation_system->recommend_by_content (*this);

}


sp_movie User::get_recommendation_by_cf(int k) const{
  return _recommendation_system->recommend_by_cf(*this,k);

}


double User::get_prediction_score_for_movie(const std::string& name, int year,
      int k) const{
  sp_movie movie = _recommendation_system->get_movie (name, year);
  return _recommendation_system->predict_movie_score (*this, movie, k);

}


std::ostream &operator<< (std::ostream &os, const User &user)
{
  os << user._username<< ": \n" << *user._recommendation_system << std::endl;
  return os;
}


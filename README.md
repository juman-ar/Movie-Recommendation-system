Movie Recommendation System
This project is part of the C/C++ Programming Workshop at the Hebrew University of Jerusalem. 
The project focuses on building a C++ library for managing a recommendation system that suggests movies to users based on their preferences. It involves implementing 
key algorithms for content-based filtering and collaborative filtering. The implementation makes use of C++ STL data structures, smart pointers, and efficient 
algorithms to handle and recommend movies to users.

Main Concepts:
Content-Based Filtering: Recommends movies that are similar to those a user has highly rated, based on the movies' features (e.g., drama, action).
Collaborative Filtering: Recommends movies based on the preferences of similar users.
STL Data Structures: use of C++ STL containers such as unordered_map to store user ratings and movie data.
Smart Pointers: Use of std::shared_ptr to manage dynamic memory allocation safely.

Features:
Movie Class: Represents a movie, including its title and year of release, and provides functionality for comparing and managing movies.
User Class: Represents users, allowing them to add and rate movies, and receive personalized recommendations based on content or collaborative filtering.
Recommendation System: The core class providing recommendations for users using the two methods—content-based filtering and collaborative filtering.
Recommendation System Loader: Loads movies and users into the recommendation system from input files.
Users Loader: Loads user information and ratings.

File Structure:
Movie.h, Movie.cpp: Definitions and implementations of the Movie class.
User.h, User.cpp: Definitions and implementations of the User class.
RecommendationSystem.h, RecommendationSystem.cpp: Handles the logic for providing recommendations.
RecommendationSystemLoader.h, RecommendationSystemLoader.cpp: Loads movies and builds the recommendation system.
UsersLoader.h, UsersLoader.cpp: Loads user data into the system.



/**
 @file Meal class header file
 @for MealTime socket programming project
 @authors Phuc T, Narissa T
 @date 3/13/22
 @version 2.0
 */
#pragma once
#include <string>

using namespace std;

class Meal
{
private:
	string name;
	string cuisine;
	string time;

public:
	Meal();

	~Meal();

	/*
	Parameterized constructor for Meal class.
	Parameters: the name of the meal
	            the time when the meal is eaten (breakfast, lunch, or dinner)
				the type of cuisine for the meal            
	*/
	Meal(string name, string cuisine, string time);

	/*
	Function:   getName
	Returns:    the cuisine type for the meal.
	*/
	string getName();
	
	/*
	Function:   getCuisine
	Returns:    the cuisine type for the meal.
	*/
	string getCuisine();

	/*
	Function:   getTime
	Returns:    the cuisine type for the meal.
	*/
	string getTime();

};


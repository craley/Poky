//only PokemonData.cpp should ever include this file. It's purely for convenience

#ifndef SQLSTMTS_HPP_
#define SQLSTMTS_HPP_

#include <string>

std::string doubleImmuneToSQL(
		 "select distinct immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?) and immunityidType is not null");

std::string doubleNormalToSQL(
		 "select idType "
		 "from Type "
		 "except "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?)");

std::string immuneToSQL(
		 "select distinct immunityidType "
		 "from Type_Immune_To "
		 "where targetidType=? and immunityidType is not null");

std::string normalToSQL(
		 "select idType "
		 "from Type "
		 "except "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=? "
		 "except "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=? "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where targetidType=?");

std::string resistantToSQL(
		 "select distinct resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=? and resistanceidType is not null");

std::string weakToSQL(
		 "select distinct weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=? and weaknessidType is not null");

std::string x2ResistantToSQL(
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where (targetidType=? or targetidType=?) and resistanceidType is not null "
		 "except "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select resistanceidType "
		 "from ( "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=? "
		 "intersect "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=?)");

std::string x2WeakToSQL(
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where (targetidType=? or targetidType=?) and weaknessidType is not null "
		 "except "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select weaknessidType "
		 "from ( "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=? "
		 "intersect "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=?)");

std::string x4ResistantToSQL(
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=? and resistanceidType is not null "
		 "intersect "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where targetidType=? "
		 "except "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?)");

std::string x4WeakToSQL(
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=? and weaknessidType is not null "
		 "intersect "
		 "select weaknessidType "
		 "from Type_Weak_To "
		 "where targetidType=? "
		 "except "
		 "select resistanceidType "
		 "from Type_Resistant_To "
		 "where (targetidType=? or targetidType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Immune_To "
		 "where (targetidType=? or targetidType=?)");

#endif /* SQLSTMTS_HPP_ */

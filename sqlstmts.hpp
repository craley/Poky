//only PokemonData.cpp should ever include this file. It's purely for convenience

#ifndef SQLSTMTS_HPP_
#define SQLSTMTS_HPP_

#include <string>

std::string doubleImmuneToSQL(
		 "select distinct immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) and immunityidType is not null");

std::string doubleNormalToSQL(
		 "select idType "
		 "from Type "
		 "except "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?)");

std::string immuneToSQL(
		 "select distinct immunityidType "
		 "from Type_Effectiveness "
		 "where idType=? and immunityidType is not null");

std::string normalToSQL(
		 "select idType "
		 "from Type "
		 "except "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "except "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where idType=?");

std::string resistantToSQL(
		 "select distinct resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=? and resistanceidType is not null");

std::string weakToSQL(
		 "select distinct weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=? and resistanceidType is not null");

std::string x2ResistantToSQL(
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) and resistanceidType is not null "
		 "except "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select resistanceidType "
		 "from ( "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "intersect "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=?)");

std::string x2WeakToSQL(
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) and resistanceidType is not null "
		 "except "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select weaknessidType "
		 "from ( "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "intersect "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=?)");

std::string x4ResistantToSQL(
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=? and resistanceidType is not null "
		 "intersect "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "except "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?)");

std::string x4WeakToSQL(
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=? and weaknessidType is not null "
		 "intersect "
		 "select weaknessidType "
		 "from Type_Effectiveness "
		 "where idType=? "
		 "except "
		 "select resistanceidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?) "
		 "except "
		 "select immunityidType "
		 "from Type_Effectiveness "
		 "where (idType=? or idType=?)");

#endif /* SQLSTMTS_HPP_ */

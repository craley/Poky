#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "PokemonData.hpp"

class PokemonData::PokemonData_
{
public:
	PokemonData_();
	~PokemonData_();

	void setPokemon(int ID);
	void setPokemon(const std::string& name);

	int getID();
	std::string getName();
	std::string getSpriteLocation();
	int getBaseHP();
	int getBaseAtt();
	int getBaseDef();
	int getBaseSpAtt();
	int getBaseSpDef();
	int getBaseSpeed();
	std::string getFlavorText();
	double getHeight();
	double getWeight();
	int getTypeID1();
	int getTypeID2();
	//if this pokemon is attacked, this is what the damage will be multiplied by
	double typeDamageMultiplier(int typeID);
	//number of pokemon in the database
	size_t numPokemon();

	std::vector<int> getPokemonWithCharacteristics(const PokemonCharacteristics& characteristics);

	std::string getTypeName(int typeID);
	int getTypeID(const std::string& type);

	void setType(int typeID1, int typeID2 = 0);
	void setType(const std::string& type1, const std::string& type2);

	std::vector<int> getTypesWeakTo();
	std::vector<int> getTypesDoubleWeakTo();
	std::vector<int> getTypesResistentTo();
	std::vector<int> getTypesDoubleResistentTo();
	std::vector<int> getTypesImmuneTo();
	std::vector<int> getTypesDamagedNormallyBy();
	size_t numTypes();

private:
	void execStmts(int ID);
	void loadSQLFile(sqlite3_stmt** stmt, const char* fileName);
	std::vector<int> getTypes(sqlite3_stmt* stmt, int numBinds);

	sqlite3* m_connection;
	sqlite3_stmt* m_pokemonStmt;
	sqlite3_stmt* m_pokemonTypeStmt;
	sqlite3_stmt* m_pokemonIDStmt;
	sqlite3_stmt* m_typeStmt;
	sqlite3_stmt* m_typeIDStmt;
	sqlite3_stmt* m_typeWeakStmt;
	sqlite3_stmt* m_typeResistStmt;
	sqlite3_stmt* m_typeImmuneStmt;
	sqlite3_stmt* m_typeNormalStmt;
	sqlite3_stmt* m_typeX2WeakStmt;
	sqlite3_stmt* m_typeX2ResistStmt;
	sqlite3_stmt* m_typeX4WeakStmt;
	sqlite3_stmt* m_typeX4ResistStmt;
	sqlite3_stmt* m_typeDoubleImmuneStmt;
	sqlite3_stmt* m_typeDoubleNormalStmt;

	int m_numPokemon;
	int m_ID;
	int m_pokemonTypeID1;
	int m_pokemonTypeID2;
	int m_numTypes;
	int m_typeID1;
	int m_typeID2;

	//just so I don't have to worry about actual column numbers
	enum
	{
		//ID, //not included because m_pokemonStmt doesn't select it
		NAME,
		SPRITE,
		BASE_HP,
		BASE_ATT,
		BASE_DEF,
		BASE_SP_ATT,
		BASE_SP_DEF,
		BASE_SPEED,
		FLAVOR_TEXT,
		HEIGHT,
		WEIGHT
	};
};

void sqliteCallBackFunc(void* foo, const char* statement)
{
	std::cout << statement << std::endl;
}

PokemonData::PokemonData_::PokemonData_()
	: m_ID(0), m_pokemonTypeID1(0), m_pokemonTypeID2(0), m_typeID1(0), m_typeID2(0)
{
	if(sqlite3_open("pokedex.ldb", &m_connection) != SQLITE_OK){
		std::cerr << "Error: Failed to open database" << std::endl;
		exit(1);
	}
	if(sqlite3_prepare_v2(
			m_connection,
			"select name, sprite, baseHP, baseAttack, baseDefense, "
			"baseSpAttack, baseSpDefense, baseSpeed, flavorText, height, weight "
			"from Pokemon where idPokemon=?",
			-1,
			&m_pokemonStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create pokemon statement" << std::endl;
		exit(1);
	}
	if(sqlite3_prepare_v2(
			m_connection,
			"select idType from Pokemon_Has_Type where idPokemon=?",
			-1,
			&m_pokemonTypeStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create pokemon type statement" << std::endl;
		exit(1);
	}
	if(sqlite3_prepare_v2(
			m_connection,
			"select idPokemon from Pokemon where name=?",
			-1,
			&m_pokemonIDStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create pokemon name statement" << std::endl;
		exit(1);
	}
	if(sqlite3_prepare_v2(
			m_connection,
			"select name from Type where idType=?",
			-1,
			&m_typeStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create type name statement" << std::endl;
		exit(1);
	}
	if(sqlite3_prepare_v2(
			m_connection,
			"select idType from Type where name=?",
			-1,
			&m_typeIDStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create typeID statement" << std::endl;
		exit(1);
	}

	loadSQLFile(&m_typeWeakStmt, "pokemon weak to.sql");
	loadSQLFile(&m_typeResistStmt, "pokemon resistant to.sql");
	loadSQLFile(&m_typeImmuneStmt, "pokemon immune to.sql");
	loadSQLFile(&m_typeNormalStmt, "pokemon normal to.sql");
	loadSQLFile(&m_typeX2WeakStmt, "pokemon x2 weak to.sql");
	loadSQLFile(&m_typeX2ResistStmt, "pokemon x2 resistant to.sql");
	loadSQLFile(&m_typeX4WeakStmt, "pokemon x4 weak to.sql");
	loadSQLFile(&m_typeX4ResistStmt, "pokemon x4 resistant to.sql");
	loadSQLFile(&m_typeDoubleImmuneStmt, "pokemon double immune to.sql");
	loadSQLFile(&m_typeDoubleNormalStmt, "pokemon double normal to.sql");

	sqlite3_stmt* numPokemonStmt;
	if(sqlite3_prepare_v2(
			m_connection,
			"select count(*) from Pokemon",
			-1,
			&numPokemonStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create pokemon name statement" << std::endl;
		exit(1);
	}
	if(sqlite3_step(numPokemonStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step numPokemonStmt" << std::endl;
		exit(1);
	}
	m_numPokemon = sqlite3_column_int(numPokemonStmt, 0);
	sqlite3_finalize(numPokemonStmt);
	numPokemonStmt = nullptr;

	sqlite3_stmt* numTypesStmt;
	if(sqlite3_prepare_v2(
			m_connection,
			"select count(*) from Type",
			-1,
			&numTypesStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to step numTypesStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_step(numTypesStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step numTypesStmt" << std::endl;
		exit(1);
	}
	m_numTypes = sqlite3_column_int(numTypesStmt, 0);
	sqlite3_finalize(numTypesStmt);
	numTypesStmt = nullptr;

	//sqlite3_trace(m_connection, sqliteCallBackFunc, NULL);
}

PokemonData::PokemonData_::~PokemonData_()
{
	sqlite3_finalize(m_pokemonStmt);
	sqlite3_finalize(m_pokemonTypeStmt);
	sqlite3_finalize(m_pokemonIDStmt);
	sqlite3_finalize(m_typeStmt);
	sqlite3_finalize(m_typeIDStmt);
	sqlite3_finalize(m_typeWeakStmt);
	sqlite3_finalize(m_typeResistStmt);
	sqlite3_finalize(m_typeImmuneStmt);
	sqlite3_finalize(m_typeNormalStmt);
	sqlite3_finalize(m_typeX2WeakStmt);
	sqlite3_finalize(m_typeX2ResistStmt);
	sqlite3_finalize(m_typeX4WeakStmt);
	sqlite3_finalize(m_typeX4ResistStmt);
	sqlite3_finalize(m_typeDoubleImmuneStmt);
	sqlite3_finalize(m_typeDoubleNormalStmt);
	sqlite3_close(m_connection);
}

void PokemonData::PokemonData_::execStmts(int id)
{
	int flag = 0;
	m_ID = id;

	sqlite3_reset(m_pokemonStmt);
	sqlite3_reset(m_pokemonTypeStmt);

	if(sqlite3_bind_int(m_pokemonStmt, 1, id) != SQLITE_OK){
		std::cerr << "Error: Failed to bind pokemonIDStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_bind_int(m_pokemonTypeStmt, 1, id) != SQLITE_OK){
		std::cerr << "Error: Failed to bind pokemonTypeStmt" << std::endl;
		exit(1);
	}

	if(sqlite3_step(m_pokemonStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step pokemonStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_step(m_pokemonTypeStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step pokemonTypeStmt" << std::endl;
		exit(1);
	}
	m_pokemonTypeID1 = sqlite3_column_int(m_pokemonTypeStmt, 0);
	if((flag = sqlite3_step(m_pokemonTypeStmt)) == SQLITE_DONE){
		m_pokemonTypeID2 = 0;
	}
	else if(flag == SQLITE_ROW){
		m_pokemonTypeID2 = sqlite3_column_int(m_pokemonTypeStmt, 0);
	}
	else{
		std::cerr << "Error: Failed to step pokemonTypeStmt a second time" << std::endl;
		exit(1);
	}
}

void PokemonData::PokemonData_::loadSQLFile(sqlite3_stmt** stmt, const char* fileName)
{
	std::fstream file;
	std::stringstream query;

	file.open(fileName, std::ios_base::in);
	if(!file.is_open()){
		std::cerr << "Error: Failed to open SQL file" << std::endl;
		exit(1);
	}
	query << file.rdbuf();
	if(sqlite3_prepare_v2(
			m_connection,
			query.str().c_str(),
			-1,
			stmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create statement from SQL file" << std::endl;
		exit(1);
	}
}

std::vector<int> PokemonData::PokemonData_::getTypes(sqlite3_stmt* stmt, int numBinds)
{
	std::vector<int> results;
	int flag;

	sqlite3_reset(stmt);
	if(m_typeID2){
		for(int i=1; i<numBinds+1; i+=2){
			if((flag = sqlite3_bind_int(stmt, i, m_typeID1)) != SQLITE_OK){
				std::cerr << "Error binding stmt in loop 1" << std::endl;
				std::cerr << flag << std::endl;
				exit(1);
			}
			if((flag = sqlite3_bind_int(stmt, i+1, m_typeID2)) != SQLITE_OK){
				std::cerr << "Error binding stmt in loop 2" << std::endl;
				std::cerr << flag << std::endl;
				exit(1);
			}
		}
	}
	else{
		for(int i=1; i<numBinds+1; i++){
			if(sqlite3_bind_int(stmt, i, m_typeID1) != SQLITE_OK){
				std::cerr << "Error binding stmt in loop" << std::endl;
				exit(1);
			}
		}
	}
	while((flag = sqlite3_step(stmt)) != SQLITE_DONE){
		if(flag == SQLITE_ROW){
			results.emplace_back(sqlite3_column_int(stmt, 0));
		}
		else{
			std::cerr << "Error stepping getTypes statement" << std::endl;
			exit(1);
		}
	}

	return results;
}

void PokemonData::PokemonData_::setPokemon(int ID)
{
	execStmts(ID);
}

void PokemonData::PokemonData_::setPokemon(const std::string& name)
{
	if(sqlite3_bind_text(m_pokemonIDStmt, 1, name.c_str(), name.size(), SQLITE_STATIC) != SQLITE_OK){
		std::cerr << "Error: Failed to bind pokemonIDStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_step(m_pokemonIDStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step pokemonIDStmt" << std::endl;
		exit(1);
	}
	execStmts(sqlite3_column_int(m_pokemonIDStmt, 0));
	sqlite3_reset(m_pokemonIDStmt);
}

int PokemonData::PokemonData_::getID()
{
	return m_ID;
}

std::string PokemonData::PokemonData_::getName()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(m_pokemonStmt, NAME));
}

std::string PokemonData::PokemonData_::getSpriteLocation()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(m_pokemonStmt, SPRITE));
}

int PokemonData::PokemonData_::getBaseHP()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_HP);
}

int PokemonData::PokemonData_::getBaseAtt()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_ATT);
}

int PokemonData::PokemonData_::getBaseDef()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_DEF);
}

int PokemonData::PokemonData_::getBaseSpAtt()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_SP_ATT);
}

int PokemonData::PokemonData_::getBaseSpDef()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_SP_DEF);
}

int PokemonData::PokemonData_::getBaseSpeed()
{
	return sqlite3_column_int(m_pokemonStmt, BASE_SPEED);
}

std::string PokemonData::PokemonData_::getFlavorText()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(m_pokemonStmt, FLAVOR_TEXT));
}

double PokemonData::PokemonData_::getHeight()
{
	return sqlite3_column_double(m_pokemonStmt, HEIGHT);
}

double PokemonData::PokemonData_::getWeight()
{
	return sqlite3_column_double(m_pokemonStmt, WEIGHT);
}

int PokemonData::PokemonData_::getTypeID1()
{
	return m_pokemonTypeID1;
}

int PokemonData::PokemonData_::getTypeID2()
{
	return m_pokemonTypeID2;
}

size_t PokemonData::PokemonData_::numPokemon()
{
	return m_numPokemon;
}

double PokemonData::PokemonData_::typeDamageMultiplier(int typeID)
{
	//TODO: actually make typeDamageMultiplier() do something
	return 0;
}

std::string PokemonData::PokemonData_::getTypeName(int typeID)
{
	sqlite3_reset(m_typeStmt);
	if(sqlite3_bind_int(m_typeStmt, 1, typeID) != SQLITE_OK){
		std::cerr << "Failed to bind m_typeStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_step(m_typeStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step typeStmt" << std::endl;
		exit(1);
	}
	return reinterpret_cast<const char*>(sqlite3_column_text(m_typeStmt, 0));
}

int PokemonData::PokemonData_::getTypeID(const std::string& type)
{
	sqlite3_reset(m_typeIDStmt);
	if(sqlite3_bind_text(m_typeIDStmt, 1, type.c_str(), type.size(), SQLITE_STATIC) != SQLITE_OK){
		std::cerr << "Failed to bind m_typeIDStmt" << std::endl;
		exit(1);
	}
	if(sqlite3_step(m_typeIDStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step typeIDStmt" << std::endl;
		exit(1);
	}
	return sqlite3_column_int(m_typeIDStmt, 0);
}

void PokemonData::PokemonData_::setType(int typeID1, int typeID2)
{
	m_typeID1 = typeID1;
	m_typeID2 = typeID2;
}

void PokemonData::PokemonData_::setType(const std::string& type1, const std::string& type2)
{
	m_typeID1 = getTypeID(type1);
	m_typeID2 = getTypeID(type2);
}

std::vector<int> PokemonData::PokemonData_::getTypesWeakTo()
{
	if(m_typeID2)
		return getTypes(m_typeX2WeakStmt, 8);
	else
		return getTypes(m_typeWeakStmt, 1);
}

std::vector<int> PokemonData::PokemonData_::getTypesDoubleWeakTo()
{
	if(m_typeID2)
		return getTypes(m_typeX4WeakStmt, 6);
	else
		return std::vector<int>();
}

std::vector<int> PokemonData::PokemonData_::getTypesResistentTo()
{
	if(m_typeID2)
		return getTypes(m_typeX2ResistStmt, 8);
	else
		return getTypes(m_typeResistStmt, 1);
}

std::vector<int> PokemonData::PokemonData_::getTypesDoubleResistentTo()
{
	if(m_typeID2)
		return getTypes(m_typeX4ResistStmt, 6);
	else
		return std::vector<int>();
}

std::vector<int> PokemonData::PokemonData_::getTypesImmuneTo()
{
	if(m_typeID2)
		return getTypes(m_typeDoubleImmuneStmt, 2);
	else
		return getTypes(m_typeImmuneStmt, 1);
}

std::vector<int> PokemonData::PokemonData_::getTypesDamagedNormallyBy()
{
	if(m_typeID2)
		return getTypes(m_typeDoubleNormalStmt, 6);
	else
		return getTypes(m_typeNormalStmt, 3);
}

PokemonData::PokemonData()
{
	impl = new PokemonData_;
}

PokemonData::~PokemonData()
{
	delete impl;
}

void PokemonData::setPokemon(int ID)
{
	impl->setPokemon(ID);
}

void PokemonData::setPokemon(const std::string& name)
{
	impl->setPokemon(name);
}

int PokemonData::getID()
{
	return impl->getID();
}

std::string PokemonData::getName()
{
	return impl->getName();
}

std::string PokemonData::getSpriteLocation()
{
	return impl->getSpriteLocation();
}

int PokemonData::getBaseHP()
{
	return impl->getBaseHP();
}

int PokemonData::getBaseAtt()
{
	return impl->getBaseAtt();
}

int PokemonData::getBaseDef()
{
	return impl->getBaseDef();
}

int PokemonData::getBaseSpAtt()
{
	return impl->getBaseSpAtt();
}

int PokemonData::getBaseSpDef()
{
	return impl->getBaseSpDef();
}

int PokemonData::getBaseSpeed()
{
	return impl->getBaseSpeed();
}

std::string PokemonData::getFlavorText()
{
	return impl->getFlavorText();
}

double PokemonData::getHeight()
{
	return impl->getHeight();
}

double PokemonData::getWeight()
{
	return impl->getWeight();
}

int PokemonData::getTypeID1()
{
	return impl->getTypeID1();
}

int PokemonData::getTypeID2()
{
	return impl->getTypeID2();
}

double PokemonData::typeDamageMultiplier(int typeID)
{
	return impl->typeDamageMultiplier(typeID);
}

size_t PokemonData::numPokemon()
{
	return impl->numPokemon();
}

std::string PokemonData::getTypeName(int typeID)
{
	return impl->getTypeName(typeID);
}

int PokemonData::getTypeID(const std::string& type)
{
	return impl->getTypeID(type);
}

void PokemonData::setType(int typeID1, int typeID2)
{
	impl->setType(typeID1, typeID2);
}

void PokemonData::setType(const std::string& type1, const std::string& type2)
{
	impl->setType(type1, type2);
}

std::vector<int> PokemonData::getTypesWeakTo()
{
	return impl->getTypesWeakTo();
}

std::vector<int> PokemonData::getTypesDoubleWeakTo()
{
	return impl->getTypesDoubleWeakTo();
}

std::vector<int> PokemonData::getTypesResistentTo()
{
	return impl->getTypesResistentTo();
}

std::vector<int> PokemonData::getTypesDoubleResistentTo()
{
	return impl->getTypesDoubleResistentTo();
}

std::vector<int> PokemonData::getTypesImmuneTo()
{
	return impl->getTypesImmuneTo();
}

std::vector<int> PokemonData::getTypesDamagedNormallyBy()
{
	return impl->getTypesDamagedNormallyBy();
}

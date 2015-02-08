#include <cstdlib>
#include <iostream>
#include <string>
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
	//std::string getSpriteLocation();
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

private:
	void execStmts(int ID);

	sqlite3* m_connection;
	sqlite3_stmt* m_pokemonStmt;
	sqlite3_stmt* m_pokemonTypeStmt;
	sqlite3_stmt* m_pokemonIDStmt;
	sqlite3_stmt* m_numPokemonStmt;

	int m_numPokemon;
	int m_ID;
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

PokemonData::PokemonData_::PokemonData_()
	: m_ID(0), m_typeID1(0), m_typeID2(0)
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
			"select count(*) from Pokemon",
			-1,
			&m_numPokemonStmt,
			nullptr) != SQLITE_OK){
		std::cerr << "Error: Failed to create pokemon name statement" << std::endl;
		exit(1);
	}
	if(sqlite3_step(m_numPokemonStmt) != SQLITE_ROW){
		std::cerr << "Error: Failed to step numPokemonStmt" << std::endl;
		exit(1);
	}
	m_numPokemon = sqlite3_column_int(m_numPokemonStmt, 0);
	sqlite3_finalize(m_numPokemonStmt);
}

PokemonData::PokemonData_::~PokemonData_()
{
	sqlite3_finalize(m_pokemonStmt);
	sqlite3_finalize(m_pokemonTypeStmt);
	sqlite3_finalize(m_pokemonIDStmt);
	sqlite3_close(m_connection);
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
	m_typeID1 = sqlite3_column_int(m_pokemonTypeStmt, 0);
	if((flag = sqlite3_step(m_pokemonTypeStmt)) == SQLITE_DONE){
		m_typeID2 = 0;
	}
	else if(flag == SQLITE_ROW){
		m_typeID2 = sqlite3_column_int(m_pokemonTypeStmt, 0);
	}
	else{
		std::cerr << "Error: Failed to step pokemonTypeStmt a second time" << std::endl;
		exit(1);
	}
}

int PokemonData::PokemonData_::getID()
{
	return m_ID;
}

std::string PokemonData::PokemonData_::getName()
{
	return reinterpret_cast<const char*>(sqlite3_column_text(m_pokemonStmt, NAME));
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
	return m_typeID1;
}

int PokemonData::PokemonData_::getTypeID2()
{
	return m_typeID2;
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

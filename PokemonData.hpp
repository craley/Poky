#ifndef POKEDEXDATA_HPP
#define POKEDEXDATA_HPP

#include <cstddef>
#include <string>

class PokemonData
{
public:
	PokemonData();
	~PokemonData();
	PokemonData(const PokemonData&) = delete;
	PokemonData& operator=(const PokemonData&) = delete;

	//set the pokemon you want the getters to work on
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
	class PokemonData_;
	PokemonData_* impl;
};

#endif

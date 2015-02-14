#pragma once

#include <cstddef>
#include <string>
#include <vector>

class PokemonData
{
public:
	struct PokemonCharacteristics;

	PokemonData();
	~PokemonData();
	PokemonData(const PokemonData&) = delete;
	PokemonData& operator=(const PokemonData&) = delete;

	//set the pokemon you want the getters to work on
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

	//std::vector<int> getPokemonWithCharacteristics(const PokemonCharacteristics& characteristics);

	std::string getTypeName(int typeID);
	int getTypeID(const std::string& type);

	void setType(int typeID1, int typeID2 = 0);
	void setType(const std::string& type1, const std::string& type2 = "");

	std::vector<int> getTypesWeakTo();
	std::vector<int> getTypesDoubleWeakTo();
	std::vector<int> getTypesResistentTo();
	std::vector<int> getTypesDoubleResistentTo();
	std::vector<int> getTypesImmuneTo();
	std::vector<int> getTypesDamagedNormallyBy();
 	//size_t numTypes();

	//a max characteristic that is lower than corresponding min characteristic will search for pokemon
	//that exactly match the min characteristic instead of searching within a range of values
	//setting a max characteristic to negative will search for pokemon from min to infinity.
	/*struct PokemonCharacteristics
	{
		PokemonCharacteristics()
			: baseHPMin(0), baseHPMax(-1), baseAttMin(0), baseAttMax(-1), baseDefMin(0), baseDefMax(-1),
			  baseSpAttMin(0), baseSpAttMax(-1), baseSpDefMin(0), baseSpDefMax(-1), baseSpeedMin(0),
			  baseSpeedMax(-1), heightMin(0), heightMax(-1), weightMin(0), weightMax(-1), typesUsingAnd(true) {}

		std::string nameSubStr;
		std::string nameStartsWith;
		int baseHPMin;
		int baseHPMax;
		int baseAttMin;
		int baseAttMax;
		int baseDefMin;
		int baseDefMax;
		int baseSpAttMin;
		int baseSpAttMax;
		int baseSpDefMin;
		int baseSpDefMax;
		int baseSpeedMin;
		int baseSpeedMax;
		double heightMin;
		double heightMax;
		double weightMin;
		double weightMax;
		std::vector<int> hasType;
		std::vector<int> isWeakTo;
		std::vector<int> isDoubleWeakTo;
		std::vector<int> isResistantTo;
		std::vector<int> isDoubleResistantTo;
		std::vector<int> isImmuneTo;
		//if true, searches for all pokemon that has every type characteristic you inputed
		//if false, searches for all pokemon that has at least one of the type characteristics you inputed
		bool typesUsingAnd;
	};*/

private:
	class PokemonData_;
	PokemonData_* impl;
};

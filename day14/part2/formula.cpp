#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <numeric> // for std::lcm()

using uint_t = unsigned long long;

struct Ingredient {
    uint_t amount;
    std::string name;
};

struct RecipeValue {
    uint_t output_amount;
    std::vector<Ingredient> inputs;
};


std::map<std::string, RecipeValue> parse_input(std::string filename)
{
    enum class State {
        Start, InNumber, InName
    };

    std::map<std::string, RecipeValue> recipes;

    std::ifstream file{filename};
    State curr_state = State::Start;
    std::string curr_token;
    RecipeValue curr_recipe_value;
    Ingredient curr_ingredient;
    bool in_inputs = true;
    while(file) {
        const char curr = file.get();
        switch(curr_state) {
        case State::Start:
            if(std::isdigit(curr)) {
                curr_state = State::InNumber;
                file.unget();
            } else if(std::isalpha(curr)) {
                curr_state = State::InName;
                file.unget();
            } else if(curr == '=') {
                file.ignore(2); // Skip "> "
                in_inputs = false;
            }
            break;
        case State::InNumber:
            if(std::isdigit(curr)) {
                curr_token += curr;
            } else {
                curr_ingredient.amount = std::stoi(curr_token);
                curr_token.clear();
                curr_state = State::Start;
            }
            break;
        case State::InName:
            if(std::isalpha(curr)) {
                curr_token += curr;
            } else {
                curr_ingredient.name = curr_token;
                curr_token.clear();
                if(in_inputs) {
                    // An another input ingredient
                    curr_recipe_value.inputs.push_back(curr_ingredient);
                } else {
                    // Add the recipe, reset
                    curr_recipe_value.output_amount = curr_ingredient.amount;
                    recipes[curr_ingredient.name] = curr_recipe_value;
                    curr_recipe_value = {};
                    in_inputs = true;
                }
                curr_state = State::Start;
            }
            break;
        }
    }

    return recipes;
}

void print(const std::map<std::string, RecipeValue> &recipes)
{
    for(const auto&[name, value] : recipes) {
        for(const auto&[amount, name] : value.inputs) {
            std::cout << amount << " " << name << ", ";
        }
        std::cout << "=> " << value.output_amount << " " << name << '\n';
    }
}

uint_t divide_round_up(uint_t a, uint_t b)
{
    return (a + b - 1) / b;
}

uint_t find_ore_needed(const std::map<std::string, RecipeValue> &recipes,
                       const Ingredient &ingredient)
{
    std::map<std::string, uint_t> leftovers;
    std::vector<Ingredient> orders;
    orders.insert(orders.begin(), ingredient);
    uint_t total_ore_needed = 0;

    while(!orders.empty()) {
        Ingredient curr_order{orders.back()};
        orders.pop_back();
        if(curr_order.name == "ORE") {
            total_ore_needed += curr_order.amount;
        } else if(auto match = leftovers.find(curr_order.name);
                  match != leftovers.end() && curr_order.amount <= match->second) {
            match->second -= curr_order.amount;
        } else {
            uint_t amount_needed = curr_order.amount - leftovers[curr_order.name];
            const RecipeValue& recipe = recipes.at(curr_order.name);
            uint_t factor = divide_round_up(amount_needed, recipe.output_amount);
            for(const auto&[input_amount, input_name] : recipe.inputs) {
                orders.push_back({input_amount * factor, input_name});
            }
            leftovers[curr_order.name] = factor * recipe.output_amount - amount_needed;
        }
    }
    return total_ore_needed;
}

int main()
{
    const std::map<std::string, RecipeValue> recipes = parse_input("input.txt");
    // Got to 3'848'998 by manually doing a binary search, digit-by-digit to
    // get as close as possible to 1 trillion in the ORE needed
    std::cout << find_ore_needed(recipes, {3'848'998, "FUEL"}) << "\n";

    return 0;
}

#define TB_IMPL
#include "local/termbox2.h"
#include <iostream>
#include <vector>
#include <algorithm> // For different vector functions (e.g. resize() or fill()).

// GLOBAL VARIABLES:

// The id helps with further logic of owning pixels on the screen, 
// Will be incremented and owned by each new sprite.
int g_sprite_id = 0;

// Helps identify items.
// Will be incremented and owned by each new item.
int g_item_id=0;

// Helps identify abilities in ability slots.
// Will be incremented and owned by each new ability in slot.
int g_ability_id=0;

// GLOBAL VARIABLES END.

// GENERAL STRUCTS:

// GENERAL FUNCTIONS:

void MakeSpriteVector2D (int sh, int sw, std::vector<std::vector<char>>& sprite, int& sprite_id){ 
    sprite_id = g_sprite_id; // Assign the object a new sprite id and increment the global sprite id for further use.
    g_sprite_id++; 
    sprite.resize(sh); // The sprite that will be loaded from another image file.

    // Allocate space for each column in each row:
    for (int i = 0; i < sh; i++){
        sprite[i].resize(sw, ('0'+sprite_id)); // Initialize each row with `sw` columns, all set to the sprite_id converted to char.
    }
}

void MakeInvSpriteVector2D(int inv_sh, int inv_sw, std::vector<std::vector<char>>& inventory_sprite, int& item_id) {
    item_id =g_item_id+1; // Make a unique item id for the created item.
    g_item_id++;

    inventory_sprite.resize(inv_sh); // The sprite that will be loaded from another image file.

    // Allocate space for each column in each row:
    for (int i = 0; i < inv_sh; i++){
        inventory_sprite[i].resize(inv_sw, ('0'+item_id)); // Initialize each row with `inv_sw` columns, all set to the item_id converted to char.
    }
}

void MakeSlotSpriteVector2D(int ssh, int ssw, std::vector<std::vector<char>>& ab_slot_sprite, int& ability_id) {
    ability_id =g_ability_id+1; // Make a unique item id for the created item.
    g_ability_id++;

    ab_slot_sprite.resize(ssh); // The sprite that will be loaded from another image file.

    // Allocate space for each column in each row:
    for (int i = 0; i < ssh; i++){
        ab_slot_sprite[i].resize(ssw, ('0'+ability_id)); // Initialize each row with `ssw` columns, all set to the ability_id converted to char.
    }
}

void PrintSprite(const std::vector<std::vector<char>>& sprite){
    for(const auto& row : sprite){
        for(const char& cell : row){
            std::cout << cell;
        }
        std::cout << "\n";
    }
}

// GENERAL FUNCTIONS END.

// Physical objects that can collide with the player(s), enemies and/or NPCs.
class Object{
    private:
        std::string name;
        std::vector<std::vector<char>> sprite;
        int sprite_width;
        int sprite_height; 
    public:
        int sprite_id = 0; 
        int position[1][1] = {0};
         
        Object(std::string n, int sh, int sw) : name(n), sprite_height(sh), sprite_width(sw){
            MakeSpriteVector2D(sh, sw, sprite, sprite_id);
        }
};

// Flying entities that move in the direction they are sent, can hit other sprites, on hit can do various things.
class Projectile { 
    private: 
        std::string name; 
        std::vector<std::vector<char>> sprite; 
        int sprite_width;
        int sprite_height;
    public:
        std::string type; // Type of projectile (for example: arrow, bullet). 
        std::string trajectory; // How the projectile travels.  
        int sprite_id = 0; // each sprite has to have its own unique id.
        int position[1][1] = {0};
        int speed = 0; // How fast the projectile travels. 
        int lifetime = 0; // How long the projectile can fly until it despawns.
        bool hit_state = 0; // Tracks if the projectile hit a target.
         
        Projectile(std::string n, int sh, int sw) : name(n), sprite_height(sh), sprite_width(sw){
            MakeSpriteVector2D(sh, sw, sprite, sprite_id);
        }
};

// Physical platforms that player(s), enemies and/or NPCs can stand on, for example, bridges, collapsible bridges, etc.
class Platform{
    private:
        std::string name; 
        std::vector<std::vector<char>> sprite; 
        int sprite_width;
        int sprite_height; 
    public: 
        int sprite_id = 0; // each sprite has to have its own unique id. 
        int position[1][1] = {0};
 
        Platform(std::string n, int sh, int sw) : name(n), sprite_height(sh), sprite_width(sw){
            MakeSpriteVector2D(sh, sw, sprite, sprite_id);
        }
};

// Interactive props like: doors, levers switches.
class Prop {
    private:
        std::string name;
        std::vector<std::vector<char>> sprite;  
        int sprite_width;
        int sprite_height; 
    public: 
        int sprite_id = 0; // each sprite has to have its own unique id. 
        int position[1][1] = {0};

        Prop(std::string n, int sh, int sw) : name(n), sprite_height(sh), sprite_width(sw){
            MakeSpriteVector2D(sh, sw, sprite, sprite_id);
        }
};

// Dangers or traps like: spikes, firepits, collapsible bridges, etc.
class Hazard {

};

// Items the player(s) or other things can interact with.
class Item {
    private:
        std::vector<std::vector<char>> sprite;  
        int sprite_width;
        int sprite_height; 
        int inventory_sprite_width;
        int inventory_sprite_height;
    public:
        std::string name; 
        int item_id=0; // Each item must have its own item id.
        std::vector<std::vector<char>> inventory_sprite; // Each item will also have an inventory sprite.
        int sprite_id = 0; // Each sprite has to have its own unique id.
        int position[1][1]={0};
        int spawnpos[1][1]={0};
        int quantity=0; 
        bool stackable=0; // Determines if the item is stackable.
        bool collected_state=0; // Determines the the item is collected or on the floor.

        Item(std::string n, int sh, int sw, int inv_sh, int inv_sw, bool stack, int q) : name(n), sprite_height(sh), sprite_width(sw), inventory_sprite_height(inv_sh), inventory_sprite_width(inv_sw), stackable(stack), quantity(q){
            MakeSpriteVector2D(sh, sw, sprite, sprite_id);
            MakeInvSpriteVector2D(inv_sh, inv_sw, inventory_sprite, item_id); 
        }
};

// Abilities that the living entities can have.
class Ability {
  private:
    int slot_sprite_width;
    int slot_sprite_height;
  public:
    std::string name; 
    int ability_id=0; // Each item must have its own item id.
    std::vector<std::vector<char>> ab_slot_sprite; // Each item will also have an inventory sprite. 
    int cooldown=0;
    int mana_cost=0;
 
  Ability(std::string n, int ssw, int ssh, int c, int mc) : name(n), slot_sprite_width(ssw), slot_sprite_height(ssh), cooldown(c), mana_cost(mc){
    MakeSlotSpriteVector2D(ssh,ssw,ab_slot_sprite,ability_id); 
  }
};

// Inventory functions for living entities with an inventory (NPCs, Enemies, Players).
class HasInventory {
  struct InventoryItem{
    std::string item_name;
    int item_id;
    std::vector<std::vector<char>> inventory_sprite;
    bool stackable=0; 
    int quantity=0;
    
    InventoryItem(const std::string& n="", int id=0, std::vector<std::vector<char>> sprite=std::vector<std::vector<char>>(), bool stack=0, int q=0) 
    : item_name(n), item_id(id), inventory_sprite(sprite), stackable(stack), quantity(q){
      if(stackable==0){
        quantity=1;
      } 
    }
  };
protected:
  std::vector<InventoryItem> inventory;
  int inventory_slots;
  int inventory_elements;
public:
  virtual ~HasInventory() = default;

  // Pure virtual functions to be implemented in derived classes
  virtual std::string getEntity() const = 0;
  virtual std::string getType() const = 0;
  virtual std::string getName() const = 0;

  HasInventory(int slots) : inventory_slots(slots), inventory_elements(0) {
    std::cout << "HasInventory constructor called with " << slots << " slots." << std::endl;
  }

  void AddItem(const std::string& item_name, int item_id, const std::vector<std::vector<char>>& inventory_sprite, bool stackable, int quantity) {
    if (inventory_elements < inventory_slots) {
      inventory.emplace_back(item_name, item_id, inventory_sprite, stackable, quantity);
      inventory_elements++;
    } else {
      std::cout << "Cannot add more items than the number of inventory slots!\n";
    }
  }

  void RemoveItem(int index) {
    if (index < 0 || index >= inventory.size()) {
      std::cout << "Invalid index!\n";
      return;
    }
    inventory.erase(inventory.begin() + index);
    inventory_elements--;
  }

  void ShowInventory() const { 
    for (const auto& item : inventory) {
      std::cout << "Entity: " << getEntity() << " / Entity name: " << getName() << " / Type: " << getType() << " --> ";
      std::cout << "Item name: " << item.item_name
      << " | Item id: " << item.item_id
      << " | Stackable: " << item.stackable
      << " | Quantity: " << item.quantity << "\n";
    }
  }
};

// Ability slots functions for living entities with an abilities (NPCs, Enemies, Players).
class HasAbilities { 
  struct AbilitiesAbility{
    std::string ability_name;
    int ability_id;
    std::vector<std::vector<char>> ability_sprite;
    float cooldown;
    float mana_cost;
 
    AbilitiesAbility(const std::string& n="", int id=0, std::vector<std::vector<char>> sprite=std::vector<std::vector<char>>(), float c=0, float mc=0) 
    : ability_name(n), ability_id(id), ability_sprite(sprite), cooldown(c), mana_cost(mc){}
  };
protected:
  std::vector<AbilitiesAbility> ability;
  int ability_slots;
  int abilities;
public:
  virtual ~HasAbilities() = default;

  // Pure virtual functions to be implemented in derived classes
  virtual std::string getEntity() const = 0;
  virtual std::string getType() const = 0;
  virtual std::string getName() const = 0;

  HasAbilities(int slots) : ability_slots(slots), abilities(0) {
    std::cout << "HasAbilities constructor called with " << slots << " slots." << std::endl;
  }

  void AddAbility(const std::string& ability_name, int ability_id, const std::vector<std::vector<char>>& ab_slot_sprite, float cooldown, float mana_cost) {
    if (abilities < ability_slots) {
      ability.emplace_back(ability_name, ability_id, ab_slot_sprite, cooldown, mana_cost);
      abilities++;
    } else {
      std::cout << "Cannot add more items than the number of inventory slots!\n";
    }
  }

  void RemoveAbility(int index) {
    if (index < 0 || index >= ability.size()) {
      std::cout << "Invalid index!\n";
      return;
    }
    ability.erase(ability.begin() + index);
    abilities--;
  }

  void ShowAbilities() const { 
    for (const auto& ab : ability) {
      std::cout << "Entity: " << getEntity() << " / Entity name: " << getName() << " / Type: " << getType() << " --> ";
      std::cout << "Name: " << ab.ability_name
      << " | Ability id: " << ab.ability_id
      << " | Cooldown: " << ab.cooldown
      << " | Mana cost: " << ab.mana_cost << "\n";
    }
  }
};

// NPCs the player(s) can interact with.
class NPC : public HasInventory, public HasAbilities {
  private:
    std::string name;
    std::string type; // Type of enemy (for example: merchant, villager, clerk).
    std::string state; // NPCs current state (for example: alive, dead, respawning).
    std::vector<std::vector<char>> sprite;  
    int sprite_width;
    int sprite_height; 
  public:
    int sprite_id = 0; // each sprite has to have its own unique id.
    int position[1][1] = {0};
    int spawnpos[1][1] = {0};
    int max_health = 0; // Maximum health.
    int health = 0; // Current health.
    int movespeed = 0;

    std::string getEntity() const override { return "NPC"; }
    std::string getType() const override { return type; }
    std::string getName() const override { return name; }

    NPC(std::string lname, std::string ltype, int sh, int sw, int inv_slots, int ab_slots) : 
    name(lname), type(ltype), sprite_height(sh), sprite_width(sw), HasInventory(inv_slots), HasAbilities(ab_slots){
      MakeSpriteVector2D(sh, sw, sprite, sprite_id); 
    }     
};

// Enemies the player(s) can interact with.
class Enemy : public HasInventory, public HasAbilities {
  private:
    std::string name;
    std::string type; // Type of enemy (for example: common, fire, boss).
    std::string state; // Enemy's current state (for example: alive, dead, respawning). 
    std::vector<std::vector<char>> sprite;  
    int sprite_width;
    int sprite_height; 
  public:
    int sprite_id = 0; // each sprite has to have its own unique id.
    int position[1][1] = {0};
    int spawnpos[1][1] = {0};
    int max_health = 0; // Maximum health.
    int health = 0; // Current health.
    int movespeed = 0;
 
    std::string getEntity() const override { return "Enemy"; }
    std::string getType() const override { return type; }
    std::string getName() const override { return name; }

    Enemy(std::string lname, std::string ltype, int sh, int sw, int inv_slots, int ab_slots) : 
    name(lname), type(ltype), sprite_height(sh), sprite_width(sw), HasInventory(inv_slots), HasAbilities(ab_slots){
      MakeSpriteVector2D(sh, sw, sprite, sprite_id); 
    }
};

// Player(s) that can be played by the user.
class Player : public HasInventory, public HasAbilities { 
  private:
    std::string name;
    std::string type; // Type of player (for example: warrior, archer, mage).
    std::string state; // Player's current state (for example: alive, dead, respawning). 
    std::vector<std::vector<char>> sprite;  
    int sprite_width;
    int sprite_height; 
  public: 
    int sprite_id=0; // each sprite has to have its own unique id.
    int position[1][1]={0};
    int max_health=0; // Maximum health.
    int health=0; // Current health.
    int movespeed=0;
 
    std::string getEntity() const override { return "Player"; }
    std::string getType() const override { return type; }
    std::string getName() const override { return name; }

    Player(std::string lname, std::string ltype, int sh, int sw, int inv_slots, int ab_slots) : 
    name(lname), type(ltype), sprite_height(sh), sprite_width(sw), HasInventory(inv_slots), HasAbilities(ab_slots){
      MakeSpriteVector2D(sh, sw, sprite, sprite_id); 
    }     
};

void checkAndInstallXdotool() {
  // Check if xdotool is installed
  if (system("command -v xdotool &> /dev/null") != 0) {
    std::cout << "xdotool is not installed. Installing...\n";
    // Install xdotool (for Debian/Ubuntu-based systems)
    system("sudo apt-get update && sudo apt-get install -y xdotool");
  } else { 
    std::cout << "xdotool is already installed.\n";
  }
}

void resetZoom() {
  // Simulate Ctrl + 0 to reset zoom to normal/default
  system("xdotool key ctrl+0");
  std::cout << "Zoom reset to normal level.\n";
}


void zoomOut(int zoomOutCount) {
  for (int i = 0; i < zoomOutCount; ++i) {
    // Simulate Ctrl + Minus to zoom out
    system("xdotool key ctrl+minus");
  }
  std::cout << "Zoomed out " << zoomOutCount << " times.\n";
}

int main(){

// Cli screen setup functions:
checkAndInstallXdotool(); // Check if xdotool is installed and if not then install it.
resetZoom(); // Reset zoom level.
zoomOut(3); // Zoom the cli screen out x amount of times.

tb_init(); // Initialize Termbox2.
// Screen height and width.
int width=tb_width();
int height=tb_height();
// Windows center coords.
int center_x=width/2;
int center_y=height/2;
// Vector to store screen info.
std::vector<std::vector<char>> screen;  
struct tb_event ev;
tb_print(center_x, center_y, TB_GREEN, TB_BLACK, "Hello World!");
tb_print(center_x, center_y+1, TB_GREEN, TB_BLACK, "Hello World!");
tb_print(center_x, center_y+2, TB_GREEN, TB_BLACK, "Hello World!");
tb_present();
tb_poll_event(&ev); // Check for an event (input event).  

tb_shutdown();

resetZoom(); // Reset zoom level.
/*
    Item Sword("Sword",5,3,8,8,0,1);
    Item Arrows("Arrows",5,5,8,8,1,64);

    Player Player_1("Denya","Mage",10,20,20,5);
    Enemy Enemy_1("Mihail","Boss",10,20,3,2);
    NPC NPC_1("Johnson","Top Villager",10,20,50,0);

    Player_1.AddItem(Sword.name, Sword.item_id, Sword.inventory_sprite, Sword.stackable, Sword.quantity);
    Player_1.AddItem(Arrows.name, Arrows.item_id, Arrows.inventory_sprite, Arrows.stackable, Arrows.quantity);
    
    Player_1.ShowInventory();
   
    Player_1.RemoveItem(1); 
    std::cout<<"\n";
    Player_1.ShowInventory();
    
    Ability Fireball("Fireball",10,10,5,10);
    Ability Eclipse("Eclipse",10,10,60,100);

    Player_1.AddAbility(Fireball.name, Fireball.ability_id, Fireball.ab_slot_sprite, Fireball.cooldown, Fireball.mana_cost);
    Player_1.AddAbility(Eclipse.name, Eclipse.ability_id, Eclipse.ab_slot_sprite, Eclipse.cooldown, Eclipse.mana_cost);
  
    Player_1.ShowAbilities();

    Player_1.RemoveAbility(0);
  
    std::cout<<"\n";
    Player_1.ShowAbilities();   

    std::cout<<"\n\n\n";

    Enemy_1.AddItem(Sword.name, Sword.item_id, Sword.inventory_sprite, Sword.stackable, Sword.quantity);
    Enemy_1.AddItem(Arrows.name, Arrows.item_id, Arrows.inventory_sprite, Arrows.stackable, Arrows.quantity);
    
    Enemy_1.ShowInventory();
   
    Enemy_1.RemoveItem(1); 
    std::cout<<"\n";
    Enemy_1.ShowInventory();
    
    Enemy_1.AddAbility(Fireball.name, Fireball.ability_id, Fireball.ab_slot_sprite, Fireball.cooldown, Fireball.mana_cost);
    Enemy_1.AddAbility(Eclipse.name, Eclipse.ability_id, Eclipse.ab_slot_sprite, Eclipse.cooldown, Eclipse.mana_cost);
  
    Enemy_1.ShowAbilities();

    Enemy_1.RemoveAbility(0);
  
    std::cout<<"\n";
    Enemy_1.ShowAbilities();   

    std::cout<<"\n\n\n";
    
    NPC_1.AddItem(Sword.name, Sword.item_id, Sword.inventory_sprite, Sword.stackable, Sword.quantity);
    NPC_1.AddItem(Arrows.name, Arrows.item_id, Arrows.inventory_sprite, Arrows.stackable, Arrows.quantity);
    
    NPC_1.ShowInventory();
   
    NPC_1.RemoveItem(1); 
    std::cout<<"\n";
    NPC_1.ShowInventory();
*/    
  return 0;
}

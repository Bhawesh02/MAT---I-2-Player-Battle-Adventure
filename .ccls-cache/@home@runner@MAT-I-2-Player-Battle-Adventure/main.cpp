

#include <iostream>
#include <memory>
#include <random>
using namespace std;
// Genertae random int between range
random_device rd;
mt19937 gen(rd());
int random(int min, int max) {
  uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

enum PlayerType { Tanky = 1, Skilled, Damage };
enum PlayerState {
  Idle = 1,
  Attacking,
  Attacked,
  Healing,
  SpecialAbility,
  Invincible
};
class Player {
private:
  int maxHealth;
  int curHealth;
  int damage;
  int damageMax;
  int healMin;
  int healMax;
  int playerNum;
  PlayerState state = Idle;
  bool isAttacked = false;
  bool isAttacking = false;
  bool isHealing = false;

public:
  Player(int pNum, int hp, int dam, int hMin, int hMax) {
    maxHealth = hp;
    curHealth = maxHealth;
    damage = dam;
    healMin = hMin;
    healMax = hMax;
    playerNum = pNum;
  }
  //--Geters
  int getHealth() { return curHealth; }
  int getMaxHealth() { return maxHealth; }
  int getDamage() { return damage; }
  int getHealMin() { return healMin; }
  int getHealMax() { return healMax; }
  int getPlayerNum() { return playerNum; }
  PlayerState getPlayerState() { return state; }
  bool IsAttacking() {
    if (state == Attacking)
      return true;
    return false;
  }
  bool IsAttacked() {
    if (state == Attacked)
      return true;
    return false;
  }
  bool IsHealing() {
    if (state == Healing)
      return true;
    return false;
  }
  bool IsUsingSpecial() {
    if (state == SpecialAbility)
      return true;
    return false;
  }
  bool IsInvincible() {
    if (state == Invincible)
      return true;
    return false;
  }
  //--Seters--
  void setHealth(int hp) { curHealth = hp; }
  void SetPlayerState(PlayerState value) { state = value; }
  virtual void specialAbility(Player &enemyPlayerr, int amt) = 0;
  virtual int additionDamage(int baseDamage) = 0;
  // Check if player is alive or not
  bool isAlive() {
    if (curHealth > 0)
      return true;
    return false;
  }
  // Deal Damage to enemy player based on damageDealth variable
  void DealDamage(int damageDealth, Player &enemyPlayer) {

    cout << "\n\nPlayer " << this->getPlayerNum() << " Dealth -> "
         << damageDealth << " damage to Player " << enemyPlayer.getPlayerNum()
         << "\n";
    if (enemyPlayer.IsInvincible()) {
      cout << "\n\nPlayer " << enemyPlayer.getPlayerNum()
           << " Special Ability activated no Damage Received\n\n";
      enemyPlayer.SetPlayerState(Idle);
      this->SetPlayerState(Idle);
      return;
    }
    // Dont do adition damage if speicial is causing damage
    if (!IsUsingSpecial()) {
      int addDamage = additionDamage(damageDealth);
      cout << "\n\nPlayer " << this->getPlayerNum()
           << " Dealth Attidional damage -> " << addDamage
           << " damage to Player " << enemyPlayer.getPlayerNum() << "\n\n";
      damageDealth += addDamage;
    }
    int enemyHealth = enemyPlayer.getHealth() - damageDealth;

    this->SetPlayerState(Attacking);
    enemyPlayer.SetPlayerState(Attacked);

    if (enemyHealth <= 0) {
      enemyPlayer.setHealth(0);
      cout << "\n\n\nPlayer " << enemyPlayer.getPlayerNum()
           << " is dead, Player " << this->getPlayerNum() << " has won!!.\n\n";
      return;
    }
    enemyPlayer.setHealth(enemyHealth);
    // cout << ",\nNow Player " << enemyPlayer.getPlayerNum() << " Health is ->
    // "
    //      << enemyPlayer.getHealth() << "\n\n\n\n";
    this->specialAbility(enemyPlayer, damageDealth);
    enemyPlayer.specialAbility(*this, damageDealth);
    if (!IsInvincible())
      SetPlayerState(Idle);
    enemyPlayer.SetPlayerState(Idle);
  }
  // heal player with healAmt , enemyPlayer is used to call special ability
  void Heal(Player &enemyPlayer, int healAmt) {
    SetPlayerState(Idle);

    if (curHealth == maxHealth) {
      cout << "\n\nRound Wasted player already full hp... NOOB\n\n";
      return;
    }
    int newHealth = curHealth + healAmt;
    if (newHealth > maxHealth)
      newHealth = maxHealth;
    this->setHealth(newHealth);
    cout << "\n\nPlayer " << this->getPlayerNum() << " is healed by " << healAmt
         << " cureent health is: " << this->getHealth() << "\n\n\n";
    this->SetPlayerState(Healing);
    this->specialAbility(enemyPlayer, healAmt);
    SetPlayerState(Idle);
  }
  virtual ~Player() {}
};

class TankyPlayer : public Player {
public:
  TankyPlayer(int pNum, int hp = 200, int dmin = 20, int hmin = 10,
              int hmax = 25)
      : Player(pNum, hp, dmin, hmin, hmax) {
    cout << "\n\nPlayer " << pNum
         << " has chossen a tanky player whose:\n\nHealth -> " << hp
         << "\nDamage -> " << dmin << "\nHeal -> " << hmin << " ~ " << hmax
         << "\n\n";
  }
  // When geting attacked return damage
  void specialAbility(Player &enemyPlayer, int amt) {

    if (!IsAttacked()) {
      return;
    }

    int randomInt = random(1, 100);
    float xDamage = 0.5;
    int activatedPercentage = 10;
    if (randomInt <= activatedPercentage) {
      SetPlayerState(SpecialAbility);

      cout << "\n\nPlayer " << getPlayerNum()
           << " Special Ability acctivated\n\n";
      DealDamage((int)(xDamage * amt), enemyPlayer);
    }
  }
  // Deals add damge between 1 and baseDamage / 3
  inline int additionDamage(int baseDamage) {
    return random(1, (int)(baseDamage / 3));
  }
};

class SkillfullPlayer : public Player {
public:
  SkillfullPlayer(int pNum, int hp = 150, int dmin = 25, int hmin = 15,
                  int hmax = 30)
      : Player(pNum, hp, dmin, hmin, hmax) {
    cout << "\n\nPlayer " << pNum
         << " has chossen a skillful player whose:\n\nHealth -> " << hp
         << "\nDamage -> " << dmin << "\nHeal -> " << hmin << " ~ " << hmax
         << "\n\n";
  }
  // When activate next round wont receive any damage
  void specialAbility(Player &enemyPlayer, int amt) {
    if (!IsAttacking()) {
      return;
    }
    int randomInt = random(1, 100);
    float xDamage = 0.5;
    int activatedPercentage = 25;
    if (randomInt <= activatedPercentage) {
      SetPlayerState(Invincible);
    }
  }
  // Deals add damge between 1 and baseDamage / 2
  inline int additionDamage(int baseDamage) {
    return random(1, (int)(baseDamage / 2));
  }
};

class DamageDealer : public Player {
public:
  DamageDealer(int pNum, int hp = 100, int dmin = 30, int hmin = 15,
               int hmax = 30)
      : Player(pNum, hp, dmin, hmin, hmax) {
    cout << "\n\nPlayer " << pNum
         << " has chossen a Damage Dealer whose:\n\nHealth -> " << hp
         << "\nDamage -> " << dmin << "\nHeal -> " << hmin << " ~ " << hmax
         << "\n\n";
  }
  // Check if healing and health is below set percentage
  void specialAbility(Player &enemyPlayer, int amt) {
    float minHealthPercentage = 0.3;
    if (!(IsHealing() && ((getHealth() - amt) <
                          ((int)(minHealthPercentage * getMaxHealth()))))) {
      return;
    }

    int randomInt = random(1, 100);
    float xHeal = 1;
    int activatedPercentage = 40;
    if (randomInt <= activatedPercentage) {
      SetPlayerState(SpecialAbility);

      cout << "Player " << getPlayerNum() << " Special Ability acctivated\n\n";
      Heal(enemyPlayer, (int)(xHeal * amt));
    }
  }
  // Deals add damge between 1 and baseDamage
  inline int additionDamage(int baseDamage) {
    return random(1, (int)(baseDamage));
  }
};

class Gameplay {
public:
  Gameplay() {
    cout
        << "Welecome to 2 Player Battle Game:\n\nRules :\n\n1) Each Palyer "
           "gets "
           "1 "
           "turn each.\n2) Each Player Type has diffeter Health , Attack "
           "Power, Heal and Special Ability\n3) The Player whose health go to "
           "0 "
           "dies and and losses .. huh noob \n4) Player will get 2 option each "
           "turn either to attack or to Heal\n5) Press H\\h to heal and D\\d "
           "to "
           "deal damage\n6)On each hit their will be additonal damagle which "
           "will differe for Type of player choosen\n\n\nThere are 3 types of "
           "player \n\n\n\n1) Tanky "
           "Player -> High "
           "Health, Low Damage, Low Healing\n  Special Ability -> There is 10% "
           "chance to deal 50% of the damage taken when hit.\n  Additon Damge "
           "-> Between 1 and BaseDamage / 3\n\n2) Skillfull "
           "Player -> Avg "
           "Health, Avg Damage, Avg "
           "Healing\n  Special Ability -> There is 25% chance to imune to "
           "damage in next after succefull attack \n  Additon "
           "Damge "
           "-> Between 1 and BaseDamage / 2\n\n3) Damage "
           "Dealer ->Low "
           "Health, High Damage, Avg "
           "Healing\n  Special Ability -> When health below 30%, 40% chance to "
           "get 2x heal\n  Additon Damge "
           "-> Between 1 and BaseDamage \n\n";
  }
  void ChooseType(int numPlayer, Player *players[numPlayer],
                  PlayerType playerType[numPlayer]) {
    int pType;
    for (int i = 0; i < numPlayer; i++) {
      int flag = 1;
      while (flag) {
        cout << "\n\nPlayer " << i + 1
             << " chosse your type (1->Tank , 2->Skilled, "
                "3->Damage)\n";
        while (!(cin >> pType) || pType < 1 || pType > 3) {
          cout << "Choose a number between 1 and 3\n";
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        flag = 0;
      }

      playerType[i] = (PlayerType)pType;
      // Make correct object

      switch (playerType[i]) {
      case Tanky:
        players[i] = new TankyPlayer(i + 1);
        break;
      case Skilled:
        players[i] = new SkillfullPlayer(i + 1);
        break;
      case Damage:
        players[i] = new DamageDealer(i + 1);

        break;
      }
    }
  }
  void Play(int numPlayer, Player *players[numPlayer]) {
    while (players[0]->isAlive() && players[1]->isAlive()) {
      int actiAmt;

      for (int i = 0; i < numPlayer; i++) {
        int curPlayerNum;
        int enemyPlayerNum;
        if (i == 0) {
          curPlayerNum = 0;
          enemyPlayerNum = 1;
        } else {
          curPlayerNum = 1;
          enemyPlayerNum = 0;
        }
        cout << "\n\nPlayer " << i + 1 << " Current Health --> "
             << players[i]->getHealth() << " \n\nPlayer " << i + 1
             << " press H to heal or D to Damage:\n";
        char in = '\0';
        while (in != 'D' && in != 'd' && in != 'H' && in != 'h') {
          cout << "Enter either D or H\n";
          cin >> in;
          in = toupper(in);
        }

        switch (in) {
        case 'D':
          actiAmt = players[curPlayerNum]->getDamage();
          players[curPlayerNum]->DealDamage(actiAmt, *players[enemyPlayerNum]);
          break;
        case 'H':
          actiAmt = random(players[curPlayerNum]->getHealMin(),
                           players[curPlayerNum]->getHealMax());
          players[curPlayerNum]->Heal(*players[enemyPlayerNum], actiAmt);
          break;
        }
        if (i == 0)
          if (!players[i + 1]->isAlive()) {
            break;
          }
      }
    }
  }
};

int main() {
  // Starting mesage
  unique_ptr<Gameplay> game(new Gameplay());

  int numPlayer = 2;
  PlayerType playerType[numPlayer];
  Player *players[numPlayer];

  // Playes choose Player type
  game->ChooseType(numPlayer, players, playerType);

  // Play game
  game->Play(numPlayer, players);
  // deallocate memory
  for (int i = 0; i < numPlayer; i++)
    delete players[i];
  return 0;
}
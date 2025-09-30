// PARA COMPILAR: g++ main.cpp levels_array.cpp -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm
#include <raylib.h>
#include <vector>
#include <math.h>
#include "levels_array.h"

// Cogumelos dourados:
bool golden_mushrooms[7] = {false, false, false, false, false, false, false};

// Chave dos níveis
bool level_unlock[8] = {false, false, false, false, false, false, false, false};

// AJuda a decidir o background:
int index;

// Posições iniciais:
Vector2 initial_positions[8] = {
    {64, 12*32},
    {64, 3*32},
    {64, 8*32},
    {64, 3*32},
    {64, 6*32},
    {3*32, 17*32},
    {32, 32},
    {0, 0}
};

// Ajudante do TileSet. Decompõe os números do mapa em coordenadas do tile.
std::vector<int> decompose(int num) {
    int row = num / 8;
    int col = num % 8;
    return {row, col};
};

// Função Que Desenha o Mapa
void draw_map(int map[20][32], Texture2D level_tile_set, Texture2D background) {
    DrawTexture(background, 0, 0, WHITE);
    for (int row=0; row<20; row++) {
        for (int col=0; col<32; col++) {
            if (map[row][col] != 0) {
                std::vector<int> tile_png = decompose(map[row][col] -1);
                Rectangle sourcerec = {static_cast<float>(tile_png[1]*32), static_cast<float>(tile_png[0]*32), 32, 32};
                Rectangle dest_rec = {static_cast<float>(col * 32), static_cast<float>(row * 32), 32, 32};
                DrawTexturePro(level_tile_set, sourcerec, dest_rec, {0, 0}, 0.0f, WHITE);
            }
        }
    }
}

// Classe do Player
class Player {
public:
    float AnimTimer = 0.0f;
    int frame = 0;
    float tx;
    float ty;
    bool isoncooldown = false;
    float timeuntil = 10.0f;
    Texture2D display = LoadTexture("sprites/cooldown_bar.png");
    Texture2D power_bar = LoadTexture("sprites/power_bar.png");
    bool OnGround = false;
    Vector2 velocity = {0, 0};
    Texture2D sprites = LoadTexture("sprites/MainMush.png");
    Texture2D swordatack = LoadTexture("sprites/sword.png");
    char animState = 'I';
    Vector2 position = {64, 12*32};
    Rectangle rect = {position.x + 2, position.y + 2, 28, 30};
    int level = 0;
    bool HasPowerUp = false;
    int powerup = 7;
    bool attacking = false;       // Se está atacando
    float attackTimer = 0.0f;     // Controle de tempo do ataque
    int attackFrame = 0;          // Frame da animação da espada
    int atack_dir = 0;
    int attack_kind = 0; // 0 - Espada; 1 - Raio;
    Rectangle attack_sword = {0, 0, 96, 96};

    // Função pra setar a posição
    void Set_position(Vector2 pos) {
        position = pos;
    }

    void Attack() {
        if (!HasPowerUp) return;
        if (!attacking) {
            if (powerup == 3) attack_kind = 0;
            if (powerup == 5) attack_kind = 1;
            attacking = true;
            attackTimer = 0.0f;
            attackFrame = 0;
        }
        switch (atack_dir) {
        case 0:
            attack_sword = {position.x + 32, position.y - 32, 96, 96};
            break;
        case 1:
            attack_sword = {position.x - 32, position.y + 32, 96, 96};
            break;
        case 2:
            attack_sword = {position.x - 96, position.y -32, 96, 96};
            break;
        case 3:
            attack_sword = {position.x -32, position.y - 96, 96, 96};
            break;
        default:
            break;
        }
    }


    // Função que desenha
    void draw() {
        // Animação
        AnimTimer += GetFrameTime();
        if (animState != 'I') {
            if (AnimTimer >= 0.1f) {
                AnimTimer = 0.0f;
                frame = (frame + 1) % 2;
            };
        } else {
            tx = 0.0f;
            ty = 0.0f;
        };
        if (animState == 'R') {
            tx = frame * 32;
            ty = 32;
        } else if (animState == 'L') {
            tx = 32 + (frame * 32);
            ty = 0;
        }

        if (attacking && powerup == 3) {
            float Angle = 0.0f;
            float origin_x = 0;
            float origin_y = 0;
            switch (atack_dir) {
            case 0:
                origin_x = position.x + 32;
                origin_y = position.y - 32;
                break;
            
            case 1:
                origin_x = position.x + 64;
                origin_y = position.y + 32;
                break;

            case 2:
                origin_x = position.x;
                origin_y = position.y + 64;
                break;
            
            case 3:
                origin_x = position.x - 32;
                origin_y = position.y;
                break;
            default:
                break;
            }
            DrawTexturePro(swordatack, {static_cast<float>(attackFrame*96), 0, 96, 96}, {origin_x, origin_y, 96, 96}, {0, 0}, 90.0f*static_cast<float>(atack_dir), WHITE);
        }
        DrawTexture(display, 4, 20*32-20, WHITE);
        if (isoncooldown) {
            int now = 10 - timeuntil;
            for (int i=0; i<now; i++) {
                DrawTexture(power_bar, i * 4 + 9, 20*32 -16, WHITE);
            }
        }
        
        // Pega do sprite
        Rectangle source = {tx, ty, 32, 32};
        Rectangle dest = {position.x, position.y, 32, 32};
        DrawTexturePro(sprites, source, dest, {0, 0}, 0.0f, WHITE);
    }

    // Função auxiliar. Vê se colide com os tiles próximos.
    bool CollideWithMap(Rectangle rec, int map[20][32]) {
        int right = (rec.x + 32) / 32;
        int left = (rec.x) / 32;
        int top = (rec.y) / 32;
        int bottom = (rec.y + 32) / 32;

        for (int row=top; row<=bottom; row++) {
            for (int col=left; col<=right; col++) {
                if (map[row][col] != 0) {
                    Rectangle tile = {static_cast<float>(col*32), static_cast<float>(row*32), 32, 32};
                    if (CheckCollisionRecs(rec, tile)) return true;
                }
            }
        }
        return false;
    }

    // Ação de Pulo
    void Jump (float jumpStrength) {
        velocity.y -= jumpStrength;
    }

    // Retorna o player pro início do nível ao tomar dano
    void take_damage() {
        position = initial_positions[level];
    }

    // Atualiza: Colisão, movimentos, Gravidade, etc.
    void update(int map[20][32], float grav) {
        // Move o player horizontalmente
        if (IsKeyDown(KEY_D)) {
            velocity.x = 4;
            animState = 'R';
        } else if (IsKeyDown(KEY_A)) {
            velocity.x = -4;
            animState = 'L';
        } else {
            velocity.x = 0;
            animState = 'I';
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            atack_dir = 0;
            Attack();
        } else if (IsKeyPressed(KEY_DOWN)) {
            atack_dir = 1;
            Attack();
            if (!isoncooldown && powerup == 3) {
                OnGround = true;
                Jump(15.0f);
                isoncooldown = true;
            };
        } else if (IsKeyPressed(KEY_LEFT)) {
            atack_dir = 2;
            Attack();
        } else if (IsKeyPressed(KEY_UP)) {
            atack_dir = 3;
            Attack();
        }


        // Ataque
        if (attacking) {
            attackTimer += GetFrameTime();
            if (attackTimer >= 0.02f) {   // tempo entre frames
                attackTimer = 0.0f;
                attackFrame++;
                if (attackFrame >= 9) {  // supondo que a espada tenha 3 frames
                    attacking = false;
                    attackFrame = 0;
                }
            }
        }

        // Cooldown
        if (isoncooldown) {
            if (timeuntil <= 0) {
                timeuntil = 10.0f;
                isoncooldown = false;
            } else {
                timeuntil -= GetFrameTime();
            }
        }

        // Pula.
        if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) && OnGround) {
            if (powerup == 0) {
                Jump(12.0f);
            } else {
                Jump(10.0f);
            }
        }

        // Colisão Horizontal
        float next_pos_x = position.x + velocity.x;
        Rectangle next_rec_x = {next_pos_x + 2, position.y + 2, 28, 30};
        if (CollideWithMap(next_rec_x, map)) {
            if (velocity.x > 0) position.x = ((int)(next_pos_x / 32)) * 32; // direita
            if (velocity.x < 0) position.x = ((int)(next_pos_x / 32) + 1) * 32; // Esq
        } else {
            position.x = next_pos_x;
        };

        // Colisão Vertical
        float next_pos_y = position.y + velocity.y;
        Rectangle next_rec_y = {position.x + 2, next_pos_y + 2, 28, 30};
        if (CollideWithMap(next_rec_y, map)) {
            if (velocity.y > 0) {
                position.y = ((int)(next_pos_y / 32)) * 32; // caindo
                OnGround = true;
            } 
            if (velocity.y < 0) {
                position.y = ((int)(next_pos_y / 32) + 1) * 32; // Subindo
                velocity.y = 0;
            }
        } else {
            position.y = next_pos_y;
            OnGround = false;
        }
        rect.x = position.x;
        rect.y = position.y;

        // Aplica a Gravidade
        if (!OnGround) {
            velocity.y += grav;
        } else {
            velocity.y = 0;
        };


        // Checa colisão com bola de fogo
    };
};

// Classe de Item
class Item {
public: // Membros
    Vector2 position;
    int kind;
    bool collected = false;
    Texture2D texture;
    Rectangle Dest = {position.x, position.y, 32, 32};

    // Construtor
    Item(Vector2 position) : position(position) {};

    // Checa colisão.
    void update(Player &p) {
        if (collected) return;
        Dest.x = position.x;
        Dest.y = position.y;
        if (CheckCollisionRecs(p.rect, Dest)) {
            collected = true;
            if (kind != 2) {
                p.HasPowerUp = true;
                p.powerup = kind;
            } else { // Power up que não ajuda no início.
                p.HasPowerUp = false;
                p.take_damage();
            };
        };
    };

    // Desenha na tela.
    void draw() {
        if (!collected) {
            Rectangle Source = {static_cast<float>(32 * kind), 0, 32, 32};
            DrawTexturePro(texture, Source, Dest, {0, 0}, 0.0f, WHITE);
        };
    };
};


Texture2D firesprite;
// Classe do tiro
class Bullet {
private:
    int vel;
    float AnimTimerF = 0.0f;
    int frameF = 0;
    
public:
    Vector2 position;
    bool fire;
    bool active = true;

    // Construtor
    Bullet(Vector2 position, int vel, bool fire) : position(position), vel(vel), fire(fire) {};

    // Atualiza o tiro.
    void update(int map[20][32], Player &p) {
        int next_tile = (position.x + vel) / 32;
        if (fire) {
            if (vel > 0) {
                next_tile = (position.x + 32 + vel) / 32;
            } else {
                next_tile = (position.x - 32 + vel) / 32;
            }
        }
        int tiley = position.y / 32;
        if (map[tiley][next_tile] != 0) {
            active = false;
        } else {
            position.x += vel;
        }
        if (fire) {
            // Atualiza a animação
            AnimTimerF += GetFrameTime();
            if (AnimTimerF >= 0.2f) {
                AnimTimerF = 0.0f;
                frameF++;
            }
            if (frameF >= 6) {
                frameF = 0;
            }

            // Checa Colisão com o player
            if (abs(p.position.x - position.x) <= 64 && abs(p.position.y - position.y) <= 64) { // Checa somente as colisões próximas
                if (CheckCollisionRecs(p.rect, {position.x, position.y + 8, 32, 16})) {
                    p.take_damage();
                    active = false;
                }
            }
        }
    }

    // Desenha o tiro
    void draw() {
        if (!active) return;
        if (!fire) {
            DrawRectangle(position.x - 2, position.y - 2, 4, 4, WHITE);
        } else {
            if (vel < 0) {
                DrawTexturePro(firesprite, {static_cast<float>(frameF*32), 0, 32, 32}, {position.x, position.y + 32, 32, 32}, {0, 0}, 180.0f, WHITE);
            } else {
                DrawTexturePro(firesprite, {static_cast<float>(frameF*32), 0, 32, 32}, {position.x, position.y, 32, 32}, {0, 0}, 0.0f, WHITE);
            }
        }
        
    }

};


// Texturas dos adversários
Texture2D textures[5];

// Vetor que guarda as balas ativas:
std::vector<Bullet> bullets;

// Classe de adversário;
class Adversary {
public:
    Vector2 position;
    Vector2 velocity;
    int type;
    bool active = true;
    Adversary(Vector2 position) : position(position) {};
    int frame = 0;
    float timer = 0.0f;
    int framescount = 1;

    void update(Player &p) {
        if (!active) return;
        timer += GetFrameTime();
        if (abs(position.x - p.position.x) <= 64 && abs(position.y - p.position.y) <= 64) {
            if (CheckCollisionRecs(p.rect, {position.x, position.y, 32, 32})) {
                if (p.velocity.y > 0) {
                    active = false;
                    p.OnGround = true;
                    p.velocity.y = 0;
                    p.Jump(5.0f);
                } else {
                    p.position = initial_positions[p.level];
                }
            }
        }
        // Colisão com tiros
        for (auto &b : bullets){
            if (CheckCollisionRecs({b.position.x -2, b.position.y -2, 4, 4}, {position.x, position.y, 32, 32}) && !b.fire) {
                b.active = false;
                active = false;
            }
        }

        // Colisão com espada
        if (p.attacking && p.attack_kind == 0) {
            if (CheckCollisionRecs(p.attack_sword, {position.x, position.y, 32, 32})) {
                active = false;
            }
        }

        // Define a quantidade de frames por animação.
        switch (p.level) {
        case 0:
            framescount = 0;
            break;

        case 1:
            framescount = 4;
            break;
        
        case 2:
            framescount = 5;
            break;

        case 3:
            framescount = 3;
            break;

        default:
            break;
        }

        if (timer >= 0.2f) {
            if (frame >= framescount) frame = -1; 
            frame++;
        }
    };
    void draw (int level){
        if (!active) return;
        if (level == 0) {
            DrawTexturePro(textures[level], {0, 0, 32, 32}, {position.x, position.y, 32, 32}, {0, 0}, 0.0f, WHITE);
        } else if (level == 1) {
            DrawTexturePro(textures[level], {static_cast<float>(frame*32), 0, 32, 32}, {position.x, position.y, 32, 32}, {0, 0}, 0.0f, WHITE);
        } else if (level == 2) {
            DrawTexturePro(textures[level], {static_cast<float>(frame*32), 0, 32, 32}, {position.x, position.y, 32, 32}, {0, 0}, 0.0f, WHITE);
        } else if (level == 3) {
            DrawTexturePro(textures[level], {0, static_cast<float>(frame*32), 64, 32}, {position.x, position.y, 64, 32}, {0, 0}, 0.0f, WHITE);
        }
    }; 
};

// Vetor de adversário
std::vector<Adversary> adversarys;

// Classe do teleporter
class TP {
public:
    Vector2 pos1, pos2;
    Texture2D sprite = LoadTexture("sprites/root.png");
    bool active = false;
    TP(Vector2 pos1, Vector2 pos2) : pos1(pos1), pos2(pos2) {};
    ~TP() {
        UnloadTexture(sprite);
    }

    void draw() {
        if (!active) return;
        DrawTexture(sprite, pos1.x, pos1.y, WHITE);
        DrawTexture(sprite, pos2.x, pos2.y, WHITE);
    }

    void update(Player &p) {
        if (!active) return;
        if(!p.HasPowerUp || p.powerup != 4) return;

        if (abs(p.position.x-pos1.x) <= 32 && abs(p.position.y - pos1.y) <= 32) {
            if (IsKeyPressed(KEY_S)) {
                p.position = pos2;
            }
        }
        if (abs(p.position.x-pos2.x) <= 32 && abs(p.position.y - pos2.y) <= 32) {
            if (IsKeyPressed(KEY_S)) {
                p.position = pos1;
            }
        }
    }

};

// Classe da saída da fase
class Exit {
private:
    Texture2D sprite = LoadTexture("sprites/exit.png");
    Texture2D key = LoadTexture("sprites/key.png");
public:
    Vector2 position = {27*32, 15*32};
    Vector2 position_key = {23*32, 11*32};

    // Descarrega as texturas
    ~Exit() {
        UnloadTexture(key);
        UnloadTexture(sprite);
    };

    // Desenha a saída
    void draw(Player &p) {
        DrawTexture(sprite, position.x, position.y, WHITE);
        if (!level_unlock[p.level]) DrawTexture(key, position_key.x, position_key.y, WHITE);
        if (!level_unlock[p.level]) DrawCircle(position.x + 32, position.y + 32, 10.0f, YELLOW);
    };

    // Checa se tocou na saída
    void CheckColisionPlayer(Player &p, Item &u, TP &t) {
        if (CheckCollisionRecs(p.rect, {position.x, position.y, 64, 64}) && level_unlock[p.level]) {
            p.level += 1; // Level seguinte

            p.Set_position(initial_positions[p.level]); // Spawn

            // Define o background
            if (p.level <= 1) {
                index = 0;
            } else if (p.level <= 4) {
                index = 1;
            } else {
                index = 2;
            };

            // Define o power up de cada level
            switch (p.level) {
                case 1:
                    u.collected = false;
                    u.kind = 1;
                    u.position = {13*32, 2*32};
                    u.texture = LoadTexture("sprites/power_up2.png");
                    position = {48, 17*32};
                    position_key = {11*32, 10*32};
                    adversarys.push_back(Adversary({4*32, 7*32}));
                    adversarys.push_back(Adversary({8*32, 15*32}));
                    adversarys.push_back(Adversary({10*32, 7*32}));
                    adversarys.push_back(Adversary({23*32, 10*32}));
                    adversarys.push_back(Adversary({28*32, 8*32}));
                    adversarys.push_back(Adversary({25*32, 12*32}));
                    adversarys.push_back(Adversary({23*32, 15*32}));
                    break;
                case 2:
                    u.collected = false;
                    u.kind = 2;
                    u.position = {17*32, 9*32};
                    u.texture = LoadTexture("sprites/power_up3.png");
                    position = {28*32, 3*32};
                    position_key = {17*32, 8*32};
                    adversarys.push_back(Adversary({14*32, 9*32}));
                    adversarys.push_back(Adversary({20*32, 9*32}));
                    adversarys.push_back(Adversary({3*32, 17*32}));
                    break;
                case 3:
                    level_unlock[3] = false;
                    u.collected = false;
                    u.kind = 3;
                    u.position = {64, 15*32};
                    u.texture = LoadTexture("sprites/power_up4.png");
                    position = {2*32, 7*32};
                    position_key = {27*32, 4*32};
                    adversarys.push_back(Adversary({18*32, 5*32}));
                    adversarys.push_back(Adversary({3*32, 11*32}));
                    adversarys.push_back(Adversary({27*32, 12*32}));
                    break;
                case 4:
                    u.collected = false;
                    u.kind = 4;
                    u.position = {30*32, 8*32};
                    u.texture = LoadTexture("sprites/power_up5.png");
                    position = {32, 16*32};
                    position_key = {11*32, 17*32};
                    t.active = true;
                    t.pos1 = {24*32, 16*32};
                    t.pos2 = {4*32, 1*32};
                    break;
                case 5:
                    u.collected = false;
                    u.kind = 5;
                    u.position = {0, 0};
                    u.texture = LoadTexture("sprites/power_up6.png");
                    position_key = {32, 6*32};
                    position = {32, 64};
                    t.active = false;
                    break;
                case 6:
                    u.collected = false;
                    u.kind = 6;
                    u.position = {0, 0};
                    u.texture = LoadTexture("sprites/power_up7.png");
                    position_key = {20*32, 15*32};
                    position = {32, 15*32};
                    break;

                case 7:
                    position_key = {27*32, 15*32};
                    position = {32, 16*32};
                    break;
                default:
                    break;
            }
        };
        if (CheckCollisionRecs(p.rect, {position_key.x, position_key.y, 64, 64}) && adversarys.size() == 0) {
            level_unlock[p.level] = true;
        }
    };
    
};



// Principal
int main() {

    // Inicia a janela
    index = 0;
    InitWindow(32*32, 32*20, "CoguRVRP");

    // Cria as instâncias
    Player player;
    Exit exit;
    Item powerup({7*32, 9*32});
    TP teleporter({0,0}, {0,0});

    // Carrega os adversários iniciais:
    adversarys.push_back(Adversary({6*32, 9*32}));

    // Cooldown de bolas de fogo
    float FireCooldown = 0.0f;

    // Carrega as texturas
    Texture2D levels_tile_group[3] = {LoadTexture("levels_1_2/tile_group.png"), LoadTexture("levels_3_4_5/tile_group.png"), LoadTexture("levels_6_7_8/tile_group.png")};
    Texture2D backgrounds[3] = {LoadTexture("sprites/back_start.png"), LoadTexture("sprites/back_middle.png"), LoadTexture("sprites/back_final.png")};
    powerup.texture = LoadTexture("sprites/power_up1.png");
    firesprite = LoadTexture("sprites/fireball.png");
    textures[0] = LoadTexture("sprites/ad1.png");
    textures[1] = LoadTexture("sprites/ad2.png");
    textures[2] = LoadTexture("sprites/ad3.png");
    textures[3] = LoadTexture("sprites/ad4.png");
    textures[4] = LoadTexture("sprites/ad5.png");

    // Power Up de pulo
    powerup.kind = 0;

    

    // Loop Principal
    SetTargetFPS(30);
    while (!WindowShouldClose()) {
        // Função de atirar
        if (IsKeyPressed(KEY_RIGHT) && player.HasPowerUp && player.powerup == 1) {
            bullets.push_back(Bullet({player.position.x + 36, player.position.y + 16}, 7, false));
        } else if (IsKeyPressed(KEY_LEFT) && player.HasPowerUp && player.powerup == 1) {
            bullets.push_back(Bullet({player.position.x - 4, player.position.y + 16}, -7, false));
        };

        // Reiniciar
        if (IsKeyPressed(KEY_R)) {
            player.Set_position(initial_positions[player.level]); // g++ main.cpp levels_array.cpp -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm
        };

        // Vê se o nível é 3
        if (player.level == 2) {
            FireCooldown += GetFrameTime();
            if (FireCooldown >= 5.0f) {
                FireCooldown = 0.0f;
                for (auto &a : adversarys) {
                    if (player.position.x - a.position.x <= 0) {
                        bullets.push_back(Bullet(a.position, -7, true));
                    } else {
                        bullets.push_back(Bullet(a.position, 7, true));
                    }
                }
            }
        }
        
        // Atualiza: Balas, Player, Power Up e saída
        for (auto &b : bullets) b.update(levels_array[player.level], player);
        player.update(levels_array[player.level], 0.5f);
        powerup.update(player);
        exit.CheckColisionPlayer(player, powerup, teleporter);
        for (auto &a : adversarys) a.update(player);
        teleporter.update(player);
        
        // Deletas as balas inativas
        for (int i = 0; i<bullets.size(); i++) {
            if (!bullets[i].active) {
                bullets.erase(bullets.begin() + i);
            }
        }

        for (int i = 0; i<adversarys.size(); i++) {
            if (!adversarys[i].active) {
                adversarys.erase(adversarys.begin() + i);
            }
        }

        //  Desenha na tela
        BeginDrawing();
        ClearBackground(WHITE);
        draw_map(levels_array[player.level], levels_tile_group[index], backgrounds[index]);
        powerup.draw();
        exit.draw(player);
        for (auto &b : bullets) b.draw();
        for (auto &a : adversarys) a.draw(player.level);
        teleporter.draw();
        player.draw();
        EndDrawing();
    }
    for (auto &s : levels_tile_group) UnloadTexture(s);
    for (auto &b : backgrounds) UnloadTexture(b);
    for (auto &a : textures) UnloadTexture(a);
    UnloadTexture(firesprite);
    UnloadTexture(player.swordatack);
    UnloadTexture(player.sprites);
    UnloadTexture(player.display);
    UnloadTexture(player.power_bar);
    CloseWindow();
}
#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include <cmath>
#include <random>
#include <ctime>
#pragma once
class Vector2f {
    friend std::ostream& operator <<(std::ostream&out, const Vector2f&obj);
public:
    float x;
    float y;
    Vector2f();
    Vector2f(float x, float y=0.0);
    Vector2f(const Vector2f&obj);
    Vector2f& operator +=(const Vector2f&rhs);
    Vector2f& operator -=(const Vector2f&rhs);
    Vector2f operator +(const Vector2f&rhs);
    Vector2f operator -(const Vector2f&rhs);
    Vector2f& operator =(const Vector2f&rhs);
    Vector2f& operator *=(float scale);
    float length_of_vector();
    Vector2f normalize();


};



class Shape{
public:
    std::shared_ptr<sf::CircleShape>shape;
    float radius;
    int shape_type;
    int fill_r;
    int fill_g;
    int fill_b;
    float opacity;
    int out_r;
    int out_g;
    int out_b;
    int outline_thickness;
Shape(float radius,int shape_type, int fill_r,int fill_g, int fill_b,float opacity, int out_r, int out_g, int out_b,int outline_thickness);
Shape(float radius, int fill_r, int fill_g, int fill_b,float opacity);
Shape(float radius,int shape_type, int fill_r,int fil_g,int fill_b,float opacity, int outline_thickness);
};



class Transform{
public :
    std::shared_ptr<Vector2f>pos_vec;
    std::shared_ptr<Vector2f>velocity_vec;
    Vector2f new_pos = Vector2f(0.0,0.0);
    float constant_distance=150.0f;
    float active_vel_x;
    float active_vel_y;
    std::string direction="none";
    float rotation;
    Transform(float pos_x, float pos_y, float rotation, float velocity_x, float velocity_y);
};






class Entity{
    public:
    std::string tag_name="Defualt";
    int entity_id=0;
    bool is_dead =false;
    bool collision=false;
    float life_span;
    bool magnetic_effect=false;
    std::shared_ptr<Transform>c_transform;
//    std::shared_ptr<Collision>m_transform;
    std::shared_ptr<Shape>c_shape;
    Entity(const std::string&tag,size_t entity_id);
    void set_small_enemy_shape(float radius, int shape_type,int fill_r,int fill_g,int fill_b,float opacity,int outline_thickness);
    void set_bullet_shape(float radius, int fill_r, int fill_g,int fill_b,float opacity);
    void set_shape(float radius,int shape_type,int fill_r,int fill_g, int fill_b,float opacity ,int out_r, int out_g, int out_b,int outline_thickness);
    void ctransform(float x, float y, float rotation, float vel_x, float vel_y);
};


typedef std::vector<std::shared_ptr<Entity>>Entityvec;
typedef std::map<std::string, Entityvec>Entitymap;


class EntityManager{
Entityvec entities;
Entityvec buffer;
Entitymap entities_mp;
int total_entities=0;
public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(std::string tag);
    void Update_entity_manager();
    Entityvec&get_entities();
    Entityvec&get_entities(const std::string&tag);
    bool is_alive(std::shared_ptr<Entity>entity);
    void destroy_entity();
};


class Game {
  EntityManager entity_manager;
  sf::RenderWindow window;
  std::shared_ptr<Entity>player;
  bool distance_value = false;
  bool paused=false;
  int data=0;
  std::string score="0";
  std::string magnetic="gravity well effect  disabled";
  std::stringstream ss;
  sf::Font font;
  sf::Text text;
  sf::Text magnetic_notification;
  bool move_y=true;
  bool move_x=true;
  bool start = true;
  bool tick = false;
  bool cool_down=false;
  bool active_bullet=false;
  float cool_down_time=0.0f;
  bool active=true;
  float duration_time=0.0f;
  float current_frame=0.0f;
  float frame_const=0.0f;
  float life_span=0.0f;
  float fps= 0.0f;
 public:
     Game();
     void set_window();
     void create_player();
     void set_score_recorder();
     void Spawn_enemy_system();
     void magnetic_pull_effect();
     void spawn_little_enemy_system(int sides, int fill_r,int fill_g, int fill_b, Vector2f posistion);
     void Collision_system();
     void checking_collision();
     void Spawn_bullet_system(float des_x, float des_y);
     void Update_life_span();
     void Update_system(std::shared_ptr<Entity>obj,std::string key);
     void Movement_system(std::vector<std::shared_ptr<Entity>>&objects);
     void Render_system();
     void frame_system(sf::Clock&clock);
     void run();
};


#endif // GEOMETRY_H_INCLUDED

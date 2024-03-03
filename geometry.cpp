#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <random>
#include <ctime>
#include <iterator>
#include <algorithm>
#include <math.h>
#include "geometry.h"
//Vector2f() default;
Vector2f::Vector2f(float x, float y)
:x{x},
 y{y}
{
}
Vector2f::Vector2f(const Vector2f&obj):x{obj.x},y{obj.y}{};

Vector2f& Vector2f::operator +=(const Vector2f&rhs){
     x+=rhs.x;
     y+=rhs.y;
     return *this;
 };
Vector2f& Vector2f::operator -=(const Vector2f&rhs){
     x-=rhs.x;
     y-=rhs.y;
     return *this;
 };
Vector2f& Vector2f::operator *=(float scale){
    this->x*=scale;
    this->y*=scale;
    return *this;
}
  Vector2f Vector2f::operator +(const Vector2f&rhs){
      Vector2f vec(x+rhs.x, y+rhs.y);
      return vec;
  };
    Vector2f Vector2f::operator -(const Vector2f&rhs){
           Vector2f vec(x-rhs.x, y-rhs.y);
           return vec;
    };
    Vector2f& Vector2f::operator=(const Vector2f&rhs){
        if(this==&rhs){
            return*this;
        }
            this->x=rhs.x;
            this->y=rhs.y;
            return *this;
    }

std::ostream& operator <<(std::ostream&out, const Vector2f&obj){
     out<<obj.x<<" "<<obj.y<<std::endl;
     return out;
  }
float Vector2f::length_of_vector(){
    float square_root=sqrt(x*x+y*y);
    return square_root;
}
  Vector2f Vector2f::normalize(){
//      float nom_x = x/sqrt(x*x+y*y);
//      float nom_y = y/sqrt(x*x+y*y);
      Vector2f nom_vec( x/sqrt(x*x+y*y),y/sqrt(x*x+y*y));
      return nom_vec;
  }


 //ENTITY CLASS
Entity::Entity(const std::string&tag,size_t entity_id)
  :tag_name      {tag},
   entity_id{entity_id}
{
}
void Entity::set_bullet_shape(float radius, int fill_r,int fill_g, int fill_b,float opacity){
    c_shape=std::make_shared<Shape>(radius,fill_r,fill_g,fill_b,opacity);
}
void Entity::set_small_enemy_shape(float radius, int shape_type,int fill_r,int fill_g,int fill_b,float opacity,int outline_thickness){
    c_shape=std::make_shared<Shape>(radius,shape_type,fill_r,fill_g,fill_b,opacity,outline_thickness);
}
 void Entity::set_shape(float radius,int shape_type,int fill_r,int fill_g, int fill_b,float opacity, int out_r, int out_g, int out_b,int outline_thickness){
     c_shape=std::make_shared<Shape>(radius,shape_type,fill_r,fill_g,fill_b,opacity,out_r,out_g,out_b,outline_thickness);
 }
  void Entity::ctransform(float x, float y, float rotation, float vel_x, float vel_y){
      c_transform=std::make_shared<Transform>(x,y,rotation, vel_x, vel_y);
  }




//ENTITY MANAGER
EntityManager::EntityManager(){};
std::shared_ptr<Entity> EntityManager::addEntity(std::string tag){
    // create an entity
    total_entities++;
    std::shared_ptr<Entity>entity=std::make_shared<Entity>(tag,total_entities);
//    entities.push_back(entity);
//    entities_mp[tag].push_back(entity);
     buffer.push_back(entity);
     return entity;
}
void EntityManager::Update_entity_manager(){
    for(auto entity:buffer){
        entities.push_back(entity);
        entities_mp[entity->tag_name].push_back(entity);
    }
    buffer.clear();
    // deleting from the main vector
    entities.erase(std::remove_if(entities.begin(),entities.end(),[](std::shared_ptr<Entity>e){return e->is_dead;}), entities.end());
    //deleting from the vector in the map
    std::map<std::string, Entityvec>::iterator it=entities_mp.begin();
    std::vector<std::string>keys;
    while(it!=entities_mp.end()){
        std::string key=it->first;
        keys.push_back(key);
        it++;
    }
    for(auto key :keys){
       entities_mp[key].erase(std::remove_if(entities_mp[key].begin(),entities_mp[key].end(),[](std::shared_ptr<Entity>e){return e->is_dead;}),entities_mp[key].end());
    }
};
Entityvec&EntityManager::get_entities(){
    return entities;
}
 Entityvec&EntityManager::get_entities(const std::string&tag){
   return entities_mp[tag];
 }



//COMPONENTS
 Shape::Shape( float radius,int shape_type, int fill_r,int fill_g, int fill_b, float opacity,int out_r, int out_g, int out_b,int outline_thickness)
  :shape_type{shape_type},
   radius{radius},
   fill_r{fill_r},
   fill_g{fill_g},
   fill_b{fill_b},
   opacity{opacity},
   out_r{out_r},
   out_g{out_g},
   out_b{out_b},
   outline_thickness{outline_thickness}
 {
    shape=std::make_shared<sf::CircleShape>(radius,shape_type);
    shape->setFillColor(sf::Color(fill_r,fill_g,fill_b,opacity));
    shape->setOutlineColor(sf::Color(out_r,out_g,out_b,opacity));
    shape->setOrigin(radius,radius);
    shape->setOutlineThickness(outline_thickness);
 };
 Shape::Shape(float radius, int fill_r, int fill_g, int fill_b,float opacity)
   :radius{radius},
   fill_r{fill_r},
   fill_g{fill_g},
   fill_b{fill_b},
   opacity{opacity}
 {
  shape=std::make_shared<sf::CircleShape>(radius);
  shape->setFillColor(sf::Color(fill_r,fill_g,fill_b,opacity));
  shape->setOrigin(radius,radius);
 }
 Shape::Shape(float radius,int shape_type, int fill_r, int fill_g, int fill_b,float opacity,int outline_thickness)
 :shape_type{shape_type},
  radius{radius},
  fill_r{fill_r},
  fill_g{fill_g},
  fill_b{fill_b},
  opacity{opacity},
  outline_thickness{outline_thickness}
 {
   shape=std::make_shared<sf::CircleShape>(radius,shape_type);
   shape->setFillColor(sf::Color(fill_r,fill_g,fill_b,opacity));
   shape->setOutlineColor(sf::Color(255,255,255,opacity));
   shape->setOutlineThickness(outline_thickness);

 }




Transform::Transform(float pos_x, float pos_y, float rotation, float velocity_x, float velocity_y )
   :rotation{rotation},
    active_vel_x{velocity_x},
    active_vel_y{velocity_y}
{
  pos_vec=std::make_shared<Vector2f>(pos_x, pos_y);
  velocity_vec= std::make_shared<Vector2f>(velocity_x,velocity_y);
}









//GAME SYSTEMS
Game::Game(){};


void Game::set_window(){
    window.create(sf::VideoMode(1200,750),"geometric wars");
    window.setFramerateLimit(20);
}





void Game::create_player(){
  player=entity_manager.addEntity("player");
  player->set_shape(50,8,0,0,0,255,255,0,0,5);
  player->ctransform(600.0,375.0,10.0,30.0,30.0);
  player->c_shape->shape->setPosition(player->c_transform->pos_vec->x,player->c_transform->pos_vec->y);
};
void Game::set_score_recorder(){
    if(!font.loadFromFile("gamefonts.ttf")){
       std::cerr<<"could not load fonts"<<std::endl;
    }
    text.setFont(font);
    text.setCharacterSize(30);
    text.setPosition(0.0,0.0);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(255,255,255));
    magnetic_notification.setFont(font);
    magnetic_notification.setCharacterSize(30);
    magnetic_notification.setStyle(sf::Text::Bold);
    magnetic_notification.setFillColor(sf::Color(255,255,255));

};





void Game::Spawn_enemy_system(){
  if(!paused){
        int x = time(0);
        srand(x);
       if(current_frame>frame_const){
//        std::cout<<current_frame<<std::endl;
          float ex= 100+rand()%1000;
          float ey= 100+rand()%550;
          int side =1+rand()%7;
          int r=rand()%255;
          int g= rand()%255;
          int b=rand()%255;
          float opacity=255.0f;
          float vel_x=1+rand()%10;
          float vel_y=1+rand()%10;
          auto enemy = entity_manager.addEntity("enemy");
          enemy->life_span=life_span;
          enemy->set_shape(30,side,r,g,b,opacity,255,255,255,3);
          enemy->ctransform(ex,ey,5.0,vel_x,vel_y);
          enemy->c_shape->shape->setPosition(enemy->c_transform->pos_vec->x,enemy->c_transform->pos_vec->y);
          current_frame=0.0f;
          tick=true;
      }
  }
}
void Game::Spawn_bullet_system(float des_x,float des_y){
   if(!paused){
            if(!active_bullet){
              auto bullet=entity_manager.addEntity("bullet");
              Vector2f destination(des_x,des_y);
              Vector2f origin(player->c_shape->shape->getPosition().x,player->c_shape->shape->getPosition().y);
              Vector2f distance=destination-origin;
              Vector2f velocity_vec=distance.normalize();
              velocity_vec*=10.0;
              bullet->set_bullet_shape(20,255,255,255,255);
              bullet->life_span=fps*40;
              bullet->ctransform(origin.x,origin.y,0.0f,velocity_vec.x,velocity_vec.y);
              bullet->c_shape->shape->setPosition(bullet->c_transform->pos_vec->x,bullet->c_transform->pos_vec->y);
          }
   }


}




void Game::Update_life_span(){
 if(!paused){
        if(tick){
            for(auto i:entity_manager.get_entities()){
               if(i->tag_name=="enemy"){
                  i->life_span-=current_frame;
                  if(i->life_span<=2000.0){
                     i->c_shape->opacity-=5.0;
        //           std::cout<<i->c_shape->opacity<<std::endl;
                     i->c_shape->shape->setFillColor(sf::Color(i->c_shape->fill_r,i->c_shape->fill_g,i->c_shape->fill_b,i->c_shape->opacity));
                     i->c_shape->shape->setOutlineColor(sf::Color(i->c_shape->out_r,i->c_shape->out_g,i->c_shape->out_b,i->c_shape->opacity));
                   }

                   if(i->c_shape->opacity<=0.0){
                      i->is_dead=true;
        //              entity_manager.destroy_entity();
                    }

                 }
              }
           }
              for(auto i:entity_manager.get_entities()){
                if(i->tag_name=="small_enemy"||i->tag_name=="bullet"){
                   i->life_span-=fps;
                   if(i->life_span<=200.0){
                       i->c_shape->opacity-=5.0;
                        i->c_shape->shape->setFillColor(sf::Color(i->c_shape->fill_r,i->c_shape->fill_g,i->c_shape->fill_b,i->c_shape->opacity));
                        if(i->tag_name=="small_enemy"){
                          i->c_shape->shape->setOutlineColor(sf::Color(255,255,255,i->c_shape->opacity));
                        }
        //
                    }
                       if(i->c_shape->opacity<=0.0){
                        i->is_dead=true;
                       }
                }

             }
   }


}




void Game::Update_system(std::shared_ptr<Entity>obj, std::string key){
    if(!paused){
         if(key=="W"){
              if((obj->c_shape->shape->getPosition().y-obj->c_shape->radius)>=150.0){
                  move_y=false;
                  distance_value= true;
                  if(obj->c_transform->direction!="upwards"){
                     obj->c_transform->velocity_vec->y*=-1.0f;
                     obj->c_transform->velocity_vec->x=0.0f;
                     if( obj->c_transform->velocity_vec->y==0.0f)
                        obj->c_transform->velocity_vec->y+=obj->c_transform->active_vel_y*-1.0f;
                    }
                   obj->c_transform->direction="upwards";
                   obj->c_transform->new_pos.x=obj->c_shape->shape->getPosition().x;
                   obj->c_transform->new_pos.y=obj->c_shape->shape->getPosition().y-obj->c_transform->constant_distance;
                }
            }
            if(key=="S"){
               if((obj->c_shape->shape->getPosition().y+obj->c_shape->radius)<=600){
                  distance_value= true;
                  if(obj->c_transform->direction!="downwards"){
                      if(move_y){
                          obj->c_transform->velocity_vec->y*=1.0f;
                          move_y=false;
                          }
                      else{
                          obj->c_transform->velocity_vec->y*=-1.0f;
                          }
                       obj->c_transform->velocity_vec->x=0.0f;
                        if( obj->c_transform->velocity_vec->y==0.0f)
                           obj->c_transform->velocity_vec->y+=obj->c_transform->active_vel_y;
                    }
                    obj->c_transform->new_pos.x=obj->c_shape->shape->getPosition().x;
                    obj->c_transform->new_pos.y=obj->c_shape->shape->getPosition().y+obj->c_transform->constant_distance;
                    obj->c_transform->direction="downwards";
                 }
            }
            if(key=="A"){
              if((obj->c_shape->shape->getPosition().x-obj->c_shape->radius)>=150.0){
                  move_x=false;
                  distance_value= true;
                  if(obj->c_transform->direction!="left"){
                      obj->c_transform->velocity_vec->y=0.0f;
                      obj->c_transform->velocity_vec->x*=-1.0f;
                      if( obj->c_transform->velocity_vec->x==0.0f)
                          obj->c_transform->velocity_vec->x+=obj->c_transform->active_vel_x*-1.0f;
                   }
                   obj->c_transform->new_pos.x=obj->c_shape->shape->getPosition().x-obj->c_transform->constant_distance;
                   obj->c_transform->new_pos.y=player->c_shape->shape->getPosition().y;
                   obj->c_transform->direction="left";
               }
            }
            if(key=="D"){
               if((obj->c_shape->shape->getPosition().x+obj->c_shape->radius)<=1050.0){
                  distance_value= true;
                  if(obj->c_transform->direction!="right"){
                     obj->c_transform->velocity_vec->y=0.0f;
                     if(move_x){
                        obj->c_transform->velocity_vec->x*=1.0f;
                        move_x=false;
                      }
                     else{
                       obj->c_transform->velocity_vec->x*=-1.0f;
                         }
                     if( obj->c_transform->velocity_vec->x==0.0f)
                         obj->c_transform->velocity_vec->x+=obj->c_transform->active_vel_x;
                   }
                   obj->c_transform->new_pos.x=obj->c_shape->shape->getPosition().x+obj->c_transform->constant_distance;
                   obj->c_transform->new_pos.y=obj->c_shape->shape->getPosition().y;
                   obj->c_transform->direction="right";
                }
            }
    }

}






void Game::Movement_system(std::vector<std::shared_ptr<Entity>>&objects){
  if(!paused){
            for(auto obj:objects){
                if(obj->tag_name=="player"){
                    obj->c_shape->shape->rotate(obj->c_transform->rotation);
                    if(distance_value){
                        obj->c_shape->shape->setPosition(obj->c_shape->shape->getPosition().x+obj->c_transform->velocity_vec->x,obj->c_shape->shape->getPosition().y+obj->c_transform->velocity_vec->y);
                       if(obj->c_shape->shape->getPosition().y==obj->c_transform->new_pos.y&&obj->c_shape->shape->getPosition().x==obj->c_transform->new_pos.x){
                           distance_value=false;
                           obj->c_transform->new_pos.x=0.0f;
                           obj->c_transform->new_pos.y=0.0f;
                        }
                      }
                  }
                if(obj->tag_name=="enemy"){
                   obj->c_shape->shape->rotate(obj->c_transform->rotation);
                   if((obj->c_shape->shape->getPosition().y+30.0)>=750||(obj->c_shape->shape->getPosition().y-30.0)<=0){
                      obj->c_transform->velocity_vec->y*=-1.0f;
                    }
                    else if((obj->c_shape->shape->getPosition().x+30.0)>=1200||(obj->c_shape->shape->getPosition().x-30.0)<=0){
                            obj->c_transform->velocity_vec->x*=-1.0f;
                    }
                   obj->c_shape->shape->setPosition(obj->c_shape->shape->getPosition().x+obj->c_transform->velocity_vec->x,obj->c_shape->shape->getPosition().y+obj->c_transform->velocity_vec->y);

                 }
                if(obj->tag_name=="bullet"||obj->tag_name=="small_enemy"){
                      obj->c_shape->shape->setPosition(obj->c_shape->shape->getPosition().x+obj->c_transform->velocity_vec->x,obj->c_shape->shape->getPosition().y+obj->c_transform->velocity_vec->y);
                 }
            }

  }
}
void Game::spawn_little_enemy_system(int sides,int fill_r, int fill_g, int fill_b, Vector2f position){
  if(!paused){
     float rotating_angle=0.0;
        for(int i=0; i<sides; i++){
           auto small_enemy=entity_manager.addEntity("small_enemy");
           small_enemy->set_small_enemy_shape(15,3,fill_r,fill_g,fill_b,255,3);
           float radian=(3.142/180)*rotating_angle;
           float vel_x=cos(radian)*2;
           float vel_y=sin(radian)*2;
           small_enemy->life_span=fps*20;
           small_enemy->ctransform(position.x,position.y,rotating_angle,vel_x,vel_y);
           small_enemy->c_shape->shape->setPosition(small_enemy->c_transform->pos_vec->x,small_enemy->c_transform->pos_vec->y);
           rotating_angle+=(360/sides);
        }
  }
}
void Game::Collision_system(){
   if(!paused){
             for(auto e:entity_manager.get_entities("enemy")){
                for(auto b:entity_manager.get_entities("bullet")){
                      Vector2f bullet_pos(b->c_shape->shape->getPosition().x,b->c_shape->shape->getPosition().y);
                      Vector2f enemy_pos(e->c_shape->shape->getPosition().x,e->c_shape->shape->getPosition().y);
                      Vector2f D_vec=enemy_pos-bullet_pos;
                      float distance=D_vec.length_of_vector();
                      if(distance<(e->c_shape->radius+b->c_shape->radius)){
                            if(b->magnetic_effect){
                                    e->is_dead=true;
                                    spawn_little_enemy_system(e->c_shape->shape_type,e->c_shape->fill_r,e->c_shape->fill_g,e->c_shape->fill_b,enemy_pos);
                                    data+=e->c_shape->shape_type;
                                    ss<<data;
                                    ss>>score;
                                    ss.clear();
                            }
                            else{
                                e->is_dead=true;
                                b->is_dead=true;
                                spawn_little_enemy_system(e->c_shape->shape_type,e->c_shape->fill_r,e->c_shape->fill_g,e->c_shape->fill_b,enemy_pos);
                                data+=e->c_shape->shape_type;
                                ss<<data;
                                ss>>score;
                                ss.clear();

                              }

                        }
                }
            }
            for(auto p:entity_manager.get_entities("player")){
                for(auto e:entity_manager.get_entities("enemy")){
                     Vector2f player_pos(p->c_shape->shape->getPosition().x,p->c_shape->shape->getPosition().y);
                      Vector2f enemy_pos(e->c_shape->shape->getPosition().x,e->c_shape->shape->getPosition().y);
                      Vector2f D_vec=player_pos-enemy_pos;
                      float distance=D_vec.length_of_vector();
                      if(distance<(p->c_shape->radius+e->c_shape->radius)){
                         e->is_dead=true;
        //                 p->c_shape->shape->setPosition(p->c_transform->pos_vec->x,p->c_transform->pos_vec->y);
                         spawn_little_enemy_system(e->c_shape->shape_type,e->c_shape->fill_r,e->c_shape->fill_g,e->c_shape->fill_b,enemy_pos);
                     }
              }
            }
   }
}

void Game::Render_system(){
    window.clear();
    text.setString("score:"+score);
    magnetic_notification.setString(magnetic);
    magnetic_notification.setPosition((window.getSize().x/2)-(magnetic_notification.getLocalBounds().width/2),0.0);
    window.draw(text);
    window.draw(magnetic_notification);
    for(auto obj:entity_manager.get_entities()){
        window.draw(*(obj->c_shape->shape));
    }

    window.display();
};



void Game::frame_system(sf::Clock&clock){
 if(!paused){
            if(start){
                 float time = clock.restart().asSeconds();
                 fps = 1.0f/(time);
                 frame_const=fps*10.0;
                 life_span=fps*4000.0;
                 start=false;
               }
               else{
                  float time = clock.restart().asSeconds();
                  fps = 1.0f/(time);
                  current_frame+=fps;
                  }
                  if(active){
                        cool_down_time+=fps;
                        if(cool_down_time>=50000.0){
                            active=false;
                            cool_down_time=0.0f;
                                if(cool_down){
                                    cool_down=false;
                                }
                                else{
                                    cool_down=true;
                                    duration_time=300.0*fps;

                                }
                          }
                  }
                  if(duration_time>0.0f){
                    duration_time-=fps;
                    if(duration_time<=0.0f){
                       active=true;
                       cool_down=false;
                       active_bullet=false;
                       magnetic="gravity well disabled";
                     }
                  }
    }




  }
void Game::magnetic_pull_effect(){
    if(!paused){
                if(cool_down){
                    for(auto b:entity_manager.get_entities("bullet")){
                        b->magnetic_effect=true;
                        for(auto e:entity_manager.get_entities("enemy")){
                            Vector2f bullet_position(b->c_shape->shape->getPosition().x,b->c_shape->shape->getPosition().y);
                            Vector2f enemy_position(e->c_shape->shape->getPosition().x,e->c_shape->shape->getPosition().y);
                            Vector2f distance_vec=enemy_position-bullet_position;
                            float distance=distance_vec.length_of_vector();
                            if(distance<=500.0){
                                   active_bullet=true;
                                   b->life_span=duration_time;
                                for (auto player:entity_manager.get_entities("player")){
                                    Vector2f player_position(player->c_shape->shape->getPosition().x,player->c_shape->shape->getPosition().y);
                                    Vector2f Dbp_vec=player_position-bullet_position;
                                    float dbp=Dbp_vec.length_of_vector();
                                        if(dbp>=500.0){
                                                magnetic="gravity well enabled";
                                                b->c_transform->velocity_vec->x=0.0;
                                                b->c_transform->velocity_vec->y=0.0;
                                                Vector2f velocity_vec=distance_vec.normalize();
                                                velocity_vec*=-10;
                                                e->c_transform->velocity_vec->x=velocity_vec.x;
                                                e->c_transform->velocity_vec->y=velocity_vec.y;
                                        }
                                }


                            }
                        }
                    }
            }

    }



}


void Game::run(){
set_window();
create_player();
set_score_recorder();
sf::Event event;
sf::Clock clock;
while(window.isOpen()){
   entity_manager.Update_entity_manager();
   Spawn_enemy_system();
  while(window.pollEvent(event)){
         if(event.type==sf::Event::MouseButtonPressed){
                if(event.mouseButton.button==sf::Mouse::Left){
                    float x=event.mouseButton.x;
                    float y=event.mouseButton.y;
                    Spawn_bullet_system(x,y);
                }
         }
         if(event.type==sf::Event::KeyPressed){
            if(event.key.code==sf::Keyboard::P){
                if(paused){
                      paused=false;
                   }
                  else {
                        paused=true;
                     };
            }
         }
  }
  auto objects = entity_manager.get_entities();
  for(auto obj:objects){
    if(obj->tag_name=="player"){
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
         Update_system(obj,"W");
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
         Update_system(obj,"S");
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
         Update_system(obj,"A");
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
         Update_system(obj,"D");
       }
     }
  }

  Movement_system(objects);
  Collision_system();
  magnetic_pull_effect();
  Render_system();
  frame_system(clock);
  Update_life_span();
  };
 }














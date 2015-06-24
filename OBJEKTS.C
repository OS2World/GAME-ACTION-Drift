// graphics of different objekts hardcoded :)
#define INCL_GPI

#include "drift.h"
#include "llist.h

// all the necessary llists
extern LLIST *asteroids;
extern LLIST *effekts;
extern LLIST *enemies;
extern LLIST *shots;
extern LLIST *extra;

extern HWND hwndClient;
extern int llist_items;
extern int nbr_asteroids;
extern int game_level;
extern int game_blackhole;
extern int game_bigspace;
extern UINT game_score;
extern int game_bigex;
extern int game_mediumex;
extern int game_smallex;
extern int game_over;
extern int game_maxshots;
extern maxthrustpixels;
extern int game_life;

// powerup vars
extern int game_shots;
extern int game_turning;
extern int game_shotlife;
extern int game_rearshot;
extern int game_shieldduration;

// extern FILE *debug;

OBJEKT *ship;

// objekt for intro en demo
OBJEKT *D;
OBJEKT *R;
OBJEKT *I;
OBJEKT *F;
OBJEKT *T;

// gonio tables
int costable[360];
int sintable[360];
int sin_shift = 10;
int cos_shift = 10;

// actual shape data
POINTL shipdata[5] = { {30, 0}, {-30, 30}, {0, 0}, {-30, -30}, {30, 0} };

POINTL asteroiddata[10];
POINTL mediumasteroid1[8];
POINTL mediumasteroid2[6];
POINTL smallasteroid[4];

POINTL satdata[8] = { {40,-30}, {-40,-30}, {0,40}, {40,-30}, {0,0},{-40,-30}, {0,0}, {0,40} };
POINTL enemy1data[5] = { {30,0}, {18,0}, {-30,30}, {-30,-30}, {18,0} };
POINTL enemy2data[7] = { {0,30}, {-30,30}, {30,0}, {-30,-30}, {0,-30}, {-30,-30}, {-30,30} };
POINTL mysterydata[8] = { {-50,0}, {-14,14}, {50,14}, {-14,50}, {-14,-50}, {50,-14}, {-14,-14}, {-50,0} };
                        
POINTL pixeldata[1] = { {0,0} };
POINTL linedata[2] = { {-20,0}, {20,0} };

POINTL shotdata[2] = { {6,0}, {26,0} };

POINTL shielddata[4] = {{60,0}, {-40,60}, {-40,-60}, {60,0} };

// power up graphics
POINTL double_shot[30] = { {30,0}, {-30,0}, {-30,0}, {0,50}, {0,50}, {30,0}, {6,6}, {6,24}, {-6,6}, {-6,24} };
POINTL long_shot[5] = { {0,50}, {30,0}, {-30,0}, {0,50}, {0,0} };
POINTL rear_shot[7] = { {0,50}, {30,0}, {0,0}, {0,14}, {0,0}, {-30,0}, {0,50} };
POINTL shield[7] =  { {30, 0}, {-30, 30}, {0, 0}, {-30, -30}, {30, 0} };
POINTL fast_shot[12] = { {-30,0}, {30,0}, {30,0}, {0,50}, {0,50}, {-30,0}, {0,50}, {0,40}, {0,30}, {0,20}, {0,10}, {0,0} };

// letters used by game
POINTL Logo_A[6] = { {10,-40}, {10,40}, {-10,40}, {-10,-40}, {-10,0}, {10,0} };
POINTL Logo_B[7] = { {-10,0}, {10,0}, {10,-40}, {-10,-40}, {-10,40}, {3,40}, {3,0} };
POINTL Logo_C[4] = { {10,40}, {-10,40}, {-10,-40}, {10,-40} };
POINTL Logo_D[5] = { {0,40}, {10,-40}, {-10,-40}, {-10,40}, {0,40} };
POINTL Logo_E[7] = { {10,40}, {-10,40}, {-10,0}, {0,0}, {-10,0}, {-10,-40}, {10,-40} };
POINTL Logo_F[5] = { {10,40}, {-10,40}, {-10,-40}, {-10,0}, {10,0} };
POINTL Logo_G[6] = { {10,40}, {-10,40}, {-10,-40}, {10,-40}, {10,0}, {0,0} };
POINTL Logo_H[6] = { {10,40}, {10,-40}, {10,0}, {-10,0}, {-10,40}, {-10,-40} };
POINTL Logo_I[2] = { {0,40}, {0,-40} };
POINTL Logo_J[3] = { {10,40}, {10,-40}, {-10,-40} };
POINTL Logo_K[6] = { {10,-40}, {-10,0}, {-10,40}, {-10,-40}, {-10,0}, {10,40} };
POINTL Logo_L[3] = { {-10,40},{-10,-40},{10,-40} };
POINTL Logo_M[5] = { {-10,-40}, {-10,40}, {0,0}, {10,40}, {10,-40} };
POINTL Logo_N[4] = { {10,40}, {10,-40}, {-10,40}, {-10,-40} };
POINTL Logo_O[5] = { {10,40}, {-10,40}, {-10,-40}, {10,-40}, {10,40} };
POINTL Logo_P[5] = { {-10,-40}, {-10,40}, {10,40}, {10,0}, {-10,0} };
POINTL Logo_Q[9] = { {10,-30}, {10,40}, {-10,40}, {-10,-30}, {3,-30}, {0,0}, {10,-40}, {3,-30}, {10,-30} };
POINTL Logo_R[6] = { {-10,-40}, {-10,40}, {0,40}, {3,0}, {-10,0}, {10,-40} };
POINTL Logo_S[6] = { {10,40}, {-10,40}, {-10,0}, {10,0}, {10,-40}, {-10,-40} };
POINTL Logo_T[4] = { {-10,40}, {10,40}, {0,40}, {0,-40} };
POINTL Logo_U[4] = { {10,40}, {10,-40}, {-10,-40}, {-10,40} };
POINTL Logo_V[3] = { {10,40}, {0,-40}, {-10,40} };
POINTL Logo_W[5] = { {10,40}, {10,-40}, {0,0}, {-10,-40}, {-10,40} };
POINTL Logo_X[5] = { {10,40}, {-10,-40}, {0,0}, {-10,40}, {10,-40} };
POINTL Logo_Y[5] = { {10,40}, {0,0}, {0,-40}, {0,0}, {-10,40} };
POINTL Logo_Z[4] = { {-10,40}, {10,40}, {-10,-40}, {10,-40} };

// special
POINTL Logo_DEV[2] = { {-10,-40} ,{10,40} };

// numbers
POINTL Logo_0[6]  = { {10,40}, {-10,40}, {-10,-40}, {10,-40}, {10,40}, {-10,-40} };
POINTL Logo_1[5]  = { {-6,30}, {0,40}, {0,-40}, {-10,-40}, {10,-40} };
POINTL Logo_2[6]  = { {-10,40}, {10,40}, {10,0}, {-10,0}, {-10,-40}, {10,-40} };
POINTL Logo_3[7]  = { {-10,40}, {10,40}, {10,0}, {-10,0}, {10,0}, {10,-40}, {-10,-40} };
POINTL Logo_4[5]  = { {-10,40}, {-10,0}, {10,0}, {10,40}, {10,-40} };
POINTL Logo_5[6]  = { {10,40}, {-10,40}, {-10,0}, {10,0}, {10,-40}, {-10,-40} };
POINTL Logo_6[6]  = { {10,40}, {-10,40}, {-10,-40}, {10,-40}, {10,0}, {-10,0} };
POINTL Logo_7[3]  = { {-10,40}, {10,40}, {10,-40} };
POINTL Logo_8[7]  = { {10,0}, {-10,0}, {-10,40}, {10,40}, {10,-40}, {-10,-40}, {-10,0} };
POINTL Logo_9[6]  = { {10,0}, {-10,0}, {-10,40}, {10,40}, {10,-40}, {-10,-40} };

OBJEKT* init_objekt (int ID, int x, int y, int angle, float speed, int draw_type, int life,LONG color, POINTL *shape, int shapesize)
{
int count;
OBJEKT* retObjekt;
retObjekt = (OBJEKT *) malloc(sizeof(OBJEKT));
if (retObjekt == NULL)
   {
   WinMessageBox (HWND_DESKTOP, hwndClient, "1) Cannot allocate memory for object!", "Drift error",0,MB_ICONEXCLAMATION | MB_OK);
   exit(EXIT_FAILURE);
   }
retObjekt->ID = ID;
retObjekt->current.x = x;
retObjekt->current.y = y;
retObjekt->old = retObjekt -> current;
retObjekt->angle = angle;
retObjekt->thrust_angle = angle;
retObjekt->thrust_speed = speed;
switch (ID)
       {
       case OT_BIGASTER:    retObjekt->rot_speed = random(8)+1;
                            break;
       case OT_MEDIUMASTER: retObjekt->rot_speed = random(15)+1;
                            break;
       case OT_SMALLASTER:  retObjekt->rot_speed = random(25)+1;
                            break;
       case OT_MYSTSHIP:    retObjekt->rot_speed = 5;
                            break;
       case OT_LINE:        retObjekt->rot_speed = random(30)+10;
                            break;
       default:             retObjekt->rot_speed = 0;
                            break;
       }
retObjekt->draw_type = draw_type;
retObjekt->life = life;
retObjekt->color = color;
retObjekt->shape = (POINTL*) malloc(sizeof(POINTL)*shapesize);
if (retObjekt->shape == NULL)
   {
   WinMessageBox (HWND_DESKTOP,hwndClient, "2) Cannot allocate memory for object!", "Drift error",0,MB_ICONEXCLAMATION | MB_OK);
   exit(EXIT_FAILURE);
   }
for (count = 0; count < shapesize; count++)
    retObjekt->shape[count] = shape[count];
retObjekt->nbr_points = shapesize;
retObjekt->notnew = FALSE;
retObjekt->real = (POINTL*) malloc(sizeof(POINTL)*shapesize);
if (retObjekt->real == NULL)
   {
   WinMessageBox (HWND_DESKTOP,hwndClient, "3) Cannot allocate memory for object!", "Drift error",0,MB_ICONEXCLAMATION | MB_OK);
   exit(EXIT_FAILURE);
   }
return(retObjekt);
}

void free_objekt (OBJEKT *objekt)
{
free(objekt -> real);
free(objekt -> shape);
free(objekt);
}

void free_all (void)
{
// free all the used llists
free_list(asteroids);
free_list(enemies);
free_list(shots);
free_list(extra);
free_list(effekts);
}

void process_effektslist (HPS hps)
{
LLIST *index;

// process the effekts llist
index = effekts;
while (index != NULL)
      {
      fast_translate_objekt(hps, index -> objekt);
      index = index -> next;
      }
}

void process_list (HPS hps)
{
LLIST *index;
LLIST *index2;
LLIST *temp;
OBJEKT *delobj;
OBJEKT *curr_obj;
int count, tmpangle, tmpangle2;
// first process the llist with the stuff that has effekt on the game
index = start;
while (index != NULL)
      {
      curr_obj=index->objekt;
      move_objekt(curr_obj);
      translate_objekt(hps, curr_obj);
      switch (curr_obj -> ID)
             {
             case OT_ENEMY2:   if (game_over)
                                  break;
                               objekt_seek(curr_obj,ship->current.x,ship->current.y,10);
                               if (random(10) == 0)
                                     {
                                     thrust_objekt( curr_obj, 3);
                                     for (count = 0; count < maxthrustpixels; count++)
                                      {
                                      tmpangle = (curr_obj->angle)+180+random(40)-20;
                                      add_list(init_objekt(OT_THRUST,curr_obj->real[0].x,curr_obj->real[0].y, tmpangle, random(8)+MAXTHRUST, DT_PIXEL, random(4)+3, CLR_YELLOW, pixeldata, 1),&effekts);
                                      }
                                     }
                               if (random(40) == 0)        // shoot
                                  {
                                   add_list(init_objekt(OT_SHOT,curr_obj->real[0].x,curr_obj->real[0].y, curr_obj -> angle, MAXTHRUST+1, DT_LINE, random(10)+30, CLR_PALEGRAY, shotdata, 2),&start);
                                   add_list(init_objekt(OT_SHOT,curr_obj->real[4].x,curr_obj->real[4].y, curr_obj -> angle, MAXTHRUST+1, DT_LINE, random(10)+30, CLR_PALEGRAY, shotdata, 2),&start);
                                  }
                               break;
             case OT_ENEMY1:   if (game_over)
                                  break;
                               objekt_seek(curr_obj,ship->current.x,ship->current.y,5);
                               if (random(20) == 0)
                                     {
                                     thrust_objekt( curr_obj, 2);
                                     for (count = 0; count < maxthrustpixels; count++)
                                      {
                                      tmpangle = (curr_obj->angle)+180+random(40)-20;
                                      add_list(init_objekt(OT_THRUST,curr_obj->real[0].x,curr_obj->real[0].y, tmpangle, random(10)+MAXTHRUST, DT_PIXEL, random(4)+3, CLR_YELLOW, pixeldata, 1),&effekts);
                                      }
                                     }
                               if (random(50) == 0)
                                   add_list(init_objekt(OT_SHOT,curr_obj->real[0].x,curr_obj->real[0].y, curr_obj -> angle, MAXTHRUST+1, DT_LINE, random(10)+30, CLR_PALEGRAY, shotdata, 2),&start);
                               break;
             case OT_MYSTSHIP: curr_obj -> angle += curr_obj -> rot_speed;
                               if (game_over)
                                  break;
                               if (random(200) < game_level && nbr_asteroids < 40)  // change rotation speed or create asteroid
                                  if (curr_obj -> rot_speed == 5) // turn faster and change color
                                     {
                                     curr_obj -> rot_speed = 30;
                                     curr_obj -> color = CLR_DARKRED;
                                     }
                                  else
                                      {
                                      curr_obj -> rot_speed = 5;
                                      curr_obj -> color = CLR_BROWN;
                                      // create some asteroids
                                      randomize_asteroids (smallasteroid,4, 50);
                                      for (count = 0; count < 4; count++)
                                          {
                                          add_list(init_objekt(OT_SMALLASTER,curr_obj->current.x,curr_obj->current.y,random(360), curr_obj->thrust_speed/2,DT_POLY,0, CLR_BROWN,smallasteroid,4),&start);
                                          nbr_asteroids++;
                                          }
                                      }
                               if (random(20) == 0)
                                  {
                                  if (curr_obj -> rot_speed == 5)
                                     thrust_objekt( curr_obj, 3);
                                  else
                                      thrust_objekt(curr_obj,10);
                                  for (count = 0; count < maxthrustpixels; count++)
                                      {
                                      tmpangle = (curr_obj->angle)+180+random(40)-20;
                                      add_list(init_objekt(OT_THRUST,curr_obj->real[0].x,curr_obj->real[0].y, tmpangle, random(10)+MAXTHRUST, DT_PIXEL, random(4)+3, CLR_YELLOW, pixeldata, 1),&effekts);
                                      }
                                  }
                               break;
             case OT_2SHOT:  if (collision(ship->current,curr_obj) || collision(ship->real[0],curr_obj))
                                {
                                game_shots = 2;
                                curr_obj->life = 3;
                                }
                              break;
             case OT_REARSHOT:
                              if (collision(ship->current,curr_obj) || collision(ship->real[0],curr_obj))
                                 {
                                 game_rearshot = TRUE;
                                 curr_obj->life = 3;
                                 }
										break;
             case OT_SHIELD:  if (game_over)
                                 break;
                              if (collision(ship->current,curr_obj) || collision(ship->real[0],curr_obj))
                                 {
                                 if (game_shieldduration == 0) // no shield active
                                    add_list(init_objekt(OT_SHIPSHIELD,ship->current.x,ship->current.y,ship->angle,0,DT_POLY,400, CLR_WHITE,shielddata,4),&effekts);
                                 game_shieldduration+=600;
                                 if (game_shieldduration > 4000)
                                    game_shieldduration = 4000;
                                 curr_obj->life = 3;
                                 curr_obj->ID = OT_DUMMY;  // really kill off shield
                                 }
										break;
				 case OT_FASTSHOT: if (collision(ship->current,curr_obj) || collision(ship->real[0],curr_obj))
											 {
											 if (game_maxshots > 4)
											  game_maxshots -= 2;
											 curr_obj->life = 3;
											 curr_obj->ID = OT_DUMMY; // really kill off fastshot
											 }
										  break;
				 case OT_LONGSHOT: if (collision(ship->current,curr_obj) || collision(ship->real[0],curr_obj))
											 {
											 game_shotlife += 4;
											 curr_obj->life = 3;
											 curr_obj->ID = OT_DUMMY; // really kill off longshot
											 }
										  break;
				 case OT_ANGRYSAT:  curr_obj -> angle +=20;
										  if (random(18) == 0)
											  {
											  // angrysat shoots A LOT
											             add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle, MAXTHRUST+6, DT_LINE, random(10)+30, CLR_RED, shotdata, 2),&start);
                                   add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle+120, MAXTHRUST+6, DT_LINE, random(10)+30, CLR_RED, shotdata, 2),&start);
                                   add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle+240, MAXTHRUST+6, DT_LINE, random(10)+30, CLR_RED, shotdata, 2),&start);
                                   }
                                if (game_bigex == game_smallex)   // low detail
                                   ;
                                   else
                                    if (game_bigex == game_mediumex) // medium detail
                                       insert_explosion (curr_obj -> current, 1);
                                    else
                                        insert_explosion (curr_obj -> current, 2);
                                break;
             case OT_SAT:       curr_obj -> angle += 5;
                                if (random(70) == 0)
                                   {
												           add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle, MAXTHRUST+2, DT_LINE, random(10)+30, CLR_CYAN, shotdata, 2),&start);
                                   add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle+120, MAXTHRUST+2, DT_LINE, random(10)+30, CLR_CYAN, shotdata, 2),&start);
                                   add_list(init_objekt(OT_SHOT,curr_obj->current.x,curr_obj->current.y, curr_obj -> angle+240, MAXTHRUST+2, DT_LINE, random(10)+30, CLR_CYAN, shotdata, 2),&start);
                                   }
                                break;
             case OT_BIGASTER:  curr_obj -> angle += curr_obj -> rot_speed;
                        		    break;
             case OT_MEDIUMASTER:     curr_obj -> angle += curr_obj->rot_speed;
                                      break;
             case OT_SMALLASTER:      curr_obj -> angle += curr_obj->rot_speed;
                                      break;
             case OT_SHIP: // check if ship has collided with asteroid
                           if (game_over)
                              break;
                           index2 = start;
                           while (index2 != NULL)
                              {
                              switch  (index2->objekt->ID) {
                                   case OT_BIGASTER:
                                   case OT_SMALLASTER:
                                   case OT_MEDIUMASTER:
                                   case OT_ENEMY1:
                                   case OT_ENEMY2:
                                   case OT_MYSTSHIP:
                                   case OT_SAT:
                                        if (index2 -> objekt -> notnew)
                                        {
                                        // check for collision
                                        if (collision(curr_obj -> real[0], index2->objekt) ||
                                           collision(curr_obj -> real[1], index2->objekt) ||
                                           collision(curr_obj -> real[3], index2->objekt) )
                                           {
                                           // ship has hit something
                                           if (game_shieldduration < 3 )
                                              {
															                curr_obj->life = 3; // terminate ship
															                game_over = TRUE;
												                      insert_explosion(curr_obj->current,game_bigex);
                                              insert_shipexplosion(curr_obj->current,5);
                                              index2->objekt->life = 3;
                                              insert_explosion(index2->objekt->current,game_bigex);
                                              }
                                           else     // destroy sats and ships on impact
                                           if (index2->objekt->ID == OT_SAT || index2->objekt->ID == OT_MYSTSHIP || index2->objekt->ID == OT_ENEMY1 || index2->objekt->ID == OT_ENEMY2)
                                            {
                                             index2->objekt->life = 3;
                                             insert_explosion(index2->objekt->current,game_smallex);
                                            }
                                            game_shieldduration -= 10;
                                           }
                                        }
                                   }
                              index2 = index2 -> next;
                              }
                           // no collision
                           break;
             case OT_SHOT: // check if shot has collided with something
                           index2 = start;
                           while (index2 != NULL)
                              {
                              switch  (index2->objekt->ID) {
                                   case OT_BIGASTER:
                                   case OT_SMALLASTER:
                                   case OT_MEDIUMASTER:
                                   case OT_SHIP:
                                   case OT_ENEMY1:
                                   case OT_ENEMY2:
                                   case OT_MYSTSHIP:
                                   case OT_SAT:
                                        if (index2 -> objekt ->life ==0 && index2 -> objekt -> notnew)
                                        {
                                        // check for collision
                                        if (collision (curr_obj -> real[1], index2 -> objekt) || collision(curr_obj->real[1],index2->objekt))
                                           {
                                           // collision !
                                           index2 -> objekt -> life = 3; // terminate life of objekt
                                           curr_obj -> life = 3;  // terminate life of shot
                                           curr_obj -> ID   = OT_DUMMY;        // make shot dummy
                                           switch (index2 -> objekt ->ID)
                                                  {
                                                  case OT_SHIP:
                                                        game_life--;
                                                        if (game_life == 0)
                                                          {
                                                          game_over = TRUE;
                                                          if (game_shieldduration > 3 ) // kills of shield (indirect)
                                                            game_shieldduration = 3;
                                                          insert_explosion(curr_obj->current,game_bigex);
                                                          insert_shipexplosion(curr_obj->current,5);
                                                          }
                                                        else
                                                          {
                                                          index2 -> objekt -> life = 0; // ship is not dead
                                                          insert_explosion(curr_obj->current,game_mediumex);
                                                          tmpangle = ship->thrust_angle;
                                                          tmpangle2 = ship->angle;
                                                          ship->thrust_angle = curr_obj->angle;
                                                          ship->angle = curr_obj->angle;
                                                          thrust_objekt(ship,15);
                                                          ship->thrust_angle = tmpangle;
                                                          ship->angle = tmpangle2+random(20)-10;
                                                          }
                                                        break;
                                                  case OT_MYSTSHIP:    insert_explosion (index2 -> objekt -> current, game_bigex);
                                                                       insert_shipexplosion (index2 -> objekt -> current, 9);
                                                                       game_score+=50;
                                                                       break;
                                                  case OT_ENEMY1:      insert_explosion (index2 -> objekt -> current, game_mediumex);
                                                                       insert_shipexplosion (index2 -> objekt -> current, 3);
                                                                       game_score+=30;
                                                                       break;
                                                  case OT_ENEMY2:      insert_explosion (index2 -> objekt -> current, game_mediumex);
                                                                       insert_shipexplosion (index2 -> objekt -> current, 5);
                                                                       game_score+=40;
                                                                       break;
                                                  case OT_SAT:         if (random(5) == 0)   // sat becomes angry
                                                                       {
                                                                       index2 -> objekt -> life = 200;  // angry sat lives only for 200 frames
                                                                       index2 -> objekt -> ID = OT_ANGRYSAT;
                                                                       index2 -> objekt -> color = CLR_RED;
                                                                       }
                                                                       if (game_bigex != game_smallex) // not low setting
                                                                            insert_shipexplosion (index2 -> objekt -> current, 6);
                                                                       insert_explosion (index2 -> objekt -> current, game_mediumex);
                                                                       game_score+=20;
                                                                       break;
                                                  case OT_BIGASTER:    insert_explosion (index2 -> objekt -> current, game_bigex);
                                                                       game_score+=5;
                                                                       explode_asteroid (index2 -> objekt -> current, index2->objekt->ID);
																							  break;
																  case OT_MEDIUMASTER: insert_explosion (index2 -> objekt -> current, game_mediumex);
																							  game_score+=10;
																							  explode_asteroid (index2 -> objekt -> current, index2->objekt->ID);
																							  break;
																  case OT_SMALLASTER:  if (game_bigex != game_smallex) // not low setting
																							  insert_explosion (index2 -> objekt -> current, game_smallex);
																							  insert_shipexplosion (index2 -> objekt -> current, 3);
																							  game_score+=15;
                        											  break;
																  }
														 }
													 // no collision
                                        }
                                        break;
                                   }
                              index2 = index2 -> next;
                              }
                           break;
             }
      if (curr_obj -> life != 0)
         curr_obj -> life--;
      temp = index -> next;
      if (curr_obj -> life == 2) // objekt almost killed
         curr_obj -> color = CLR_BLACK;
      if (curr_obj -> life == 1)
         {
         switch (curr_obj -> ID)
                {
                case OT_SMALLASTER:
                case OT_MEDIUMASTER:
                case OT_BIGASTER:
                case OT_MYSTSHIP:
                case OT_ENEMY1:
                case OT_ENEMY2:
                case OT_SAT:
                case OT_ANGRYSAT:   nbr_asteroids--;
                                    break;
                }
         delobj = curr_obj;
         delete_list (curr_obj,&start);
         free_objekt(delobj);
         }
      index = temp;
      }
// then process the effekts llist
index = effekts;
while (index != NULL)
      {
      switch(index -> objekt -> ID)
                   {
                   case OT_THRUST:    if (index->objekt->life < 5 && index->objekt->life>2)
                                        index->objekt->color = CLR_RED;
                                      break;                   case OT_LINE:      index -> objekt -> angle += index -> objekt -> rot_speed;
                                      break;
                   case OT_SHIPSHIELD:index -> objekt -> current = ship->current;
                                      index -> objekt -> angle = ship->angle;
                                      if (game_shieldduration > 3)
                                         {
                                         index -> objekt -> life = game_shieldduration;
                                               if (game_shieldduration < 256)
                                                  {
                                                  if (game_shieldduration < 128)
                                                    index -> objekt -> color = CLR_BROWN;
                                                  else
                                                  	index -> objekt -> color = CLR_YELLOW;
                                               		}
                                               else
                                                  index -> objekt -> color =CLR_WHITE;
                                         }
                                      if (game_shieldduration == 3)
                                         {
                                         index -> objekt -> life = 3; // kills of shield
                                         index -> objekt -> ID = OT_DUMMY;
                                         }
                                      break;
                   }
      move_objekt(index -> objekt);
      translate_objekt(hps, index -> objekt);
      if (index -> objekt -> life != 0)
         index -> objekt -> life--;
      temp = index -> next;
      if (index -> objekt -> life == 2) // objekt almost killed
         index -> objekt -> color = CLR_BLACK;
      if (index -> objekt -> life == 1)
         {
         delobj = index -> objekt;
         delete_list (index -> objekt,&effekts);
         free_objekt(delobj);
         }
      index = temp;
      }
}

void process_list_zoom (HPS hps, int zoom)
{
LLIST *index;
LLIST *temp;
OBJEKT *delobj;

index = effekts;  // first process the effekts llist
while (index != NULL)
      {
      index -> objekt -> zoom = zoom;
      move_objekt(index -> objekt);
      translate_zoom_objekt(hps, index -> objekt);
      if (index -> objekt -> life != 0)
         index -> objekt -> life--;
      temp = index -> next;
      if (index -> objekt -> life == 2) // objekt almost killed
         index -> objekt -> color = CLR_BLACK;
      if (index -> objekt -> life == 1)
         {
         delobj = index -> objekt;
         delete_list (index -> objekt,&effekts);
         free_objekt(delobj);
         }
      index = temp;
      }

index = start;
while (index != NULL)
      {
      index -> objekt -> zoom = zoom;
      move_objekt(index -> objekt);
      translate_zoom_objekt(hps, index -> objekt);
      switch (index -> objekt -> ID)
             {
             case OT_BIGASTER:  index -> objekt -> angle += index -> objekt -> rot_speed;
                        		break;
             }
      if (index -> objekt -> life != 0)
         index -> objekt -> life--;
      temp = index -> next;
      if (index -> objekt -> life == 2) // objekt almost killed
         index -> objekt -> color = CLR_BLACK;
      if (index -> objekt -> life == 1)
         {
         delobj = index -> objekt;
         delete_list (index -> objekt, &start);
         free_objekt(delobj);
         }
      index = temp;
      }
}

void init_objekts (void)
{
// real objekt for the LLIST
ship = init_objekt (OT_SHIP,1000,1000,0,0,DT_POLY,0,CLR_RED,shipdata,5);
}

void init_logo (void)
{
D = init_objekt(999,600,1000,0,0,DT_POLY,0,CLR_BLUE,Logo_D,sizeof(Logo_D)/sizeof(POINTL));
R = init_objekt(999,800,1000,0,0,DT_POLY,0,CLR_BLUE,Logo_R,sizeof(Logo_R)/sizeof(POINTL));
I = init_objekt(999,1000,1000,0,0,DT_POLY,0,CLR_BLUE,Logo_I,sizeof(Logo_I)/sizeof(POINTL));
F = init_objekt(999,1200,1000,0,0,DT_POLY,0,CLR_BLUE,Logo_F,sizeof(Logo_F)/sizeof(POINTL));
T = init_objekt(999,1400,1000,0,0,DT_POLY,0,CLR_BLUE,Logo_T,sizeof(Logo_T)/sizeof(POINTL));
}

void free_demo (void)
{
free_objekt(D);
free_objekt(R);
free_objekt(I);
free_objekt(F);
free_objekt(T);
}

void thrust_objekt (OBJEKT *objekt, int thrust)
{
float Tr_a,Tr_b;  // needed for thrustvector calculation

Tr_a = objekt->thrust_speed*cos(to_rad(objekt->thrust_angle))+thrust*cos(to_rad(objekt->angle));
Tr_b = objekt->thrust_speed*sin(to_rad(objekt->thrust_angle))+thrust*sin(to_rad(objekt->angle));
objekt->thrust_speed = sqrt((Tr_a*Tr_a)+(Tr_b*Tr_b));
if (Tr_a == 0)
   objekt->thrust_angle = objekt->angle;
else
    objekt->thrust_angle = to_deg(atan(Tr_b/Tr_a));
if (Tr_a < 0)
   objekt->thrust_angle += 180;
if (objekt->thrust_speed > MAXTHRUST)
   objekt->thrust_speed = MAXTHRUST;
}

void fast_translate_objekt (HPS hps, OBJEKT *objekt)
{
int count, x_rot, y_rot, x_temp;
POINTL *pShape;
pShape = objekt->shape;
   objekt->old = objekt->current;
   GpiSetColor(hps,objekt->color);//objekt->color);
   for (count = 0; count < objekt->nbr_points; count++, pShape++)
          {
          objekt->real[count].x = objekt->shape[count].x+ objekt->current.x;
          objekt->real[count].y = objekt->shape[count].y+ objekt->current.y;
          }
   switch (objekt->draw_type)
       {
       case DT_POLY : GpiMove(hps,&(objekt->real[0]));
       								GpiPolyLine (hps, objekt->nbr_points,objekt->real);
                      break;
       case DT_LINE : for (count = 0; count < objekt->nbr_points/2; count++)
                        {
                        GpiMove(hps,&(objekt->real[count*2]));
                        GpiPolyLine(hps,2,&(objekt->real[count*2]));
                        }//GpiPolyLine (hps, 2,pReal);
       								break;
       case DT_PIXEL: GpiSetPel (hps, &(objekt -> current));
                      break;
       }
}

void translate_objekt (HPS hps, OBJEKT *objekt)
{
int count, x_rot, y_rot, x_temp;
POINTL *pShape;
pShape = objekt->shape;
   // remove old shape
   GpiSetColor(hps,CLR_BLACK);
   switch (objekt->draw_type)
		 {
       case DT_POLY : GpiMove(hps,&(objekt->real[0]));
       								GpiPolyLine (hps, objekt->nbr_points,objekt->real);
                      break;
       case DT_LINE : for (count = 0; count < objekt->nbr_points/2; count++)
                        {
                        GpiMove(hps,&(objekt->real[count*2]));
                        GpiPolyLine(hps,2,&(objekt->real[count*2]));
                        }
                      break;
       case DT_PIXEL: GpiSetPel (hps, &(objekt -> old));
                      break;
       }
   // rotation
   if (objekt->draw_type != DT_PIXEL || (objekt->ID == OT_SHOT && objekt->notnew == TRUE)  // powerups need no rotation
       || objekt->ID != OT_2SHOT)                                                          // pixel needs no rotation
      {                                                                                    // shot needs only 1 rotation
      if (game_blackhole == TRUE) // Oho! Black hole level
         if ((objekt->ID >= OT_BIGASTER) && (objekt->ID <= OT_SMALLASTER)) // objekt is asteroid
            {
            sin_shift = 9;
            cos_shift = 11;
            }
      if (game_bigspace == TRUE && objekt->ID != OT_LOGO) // Yesss... BIG space
        {
        sin_shift = 11;
        cos_shift = 11;
        }
      if (objekt->angle < 0)
         objekt->angle += 360;;
      if (objekt->angle >= 360 )
         objekt->angle -= 360;
      for (count = 0; count < objekt->nbr_points; count++, pShape++)
			 {
          x_rot =objekt->shape[count].x;
          y_rot =objekt->shape[count].y;
          x_temp = x_rot;
          x_rot = i_cos(x_rot,objekt->angle)-i_sin(y_rot,objekt->angle);
          y_rot = i_sin(x_temp,objekt->angle)+i_cos(y_rot,objekt->angle);
          objekt->real[count].x = x_rot + objekt->current.x;
			 objekt->real[count].y = y_rot + objekt->current.y;
          }
      if (sin_shift != 10)
         sin_shift = 10;
      if (cos_shift != 10)
         cos_shift = 10;
      }
   objekt->notnew = TRUE; // real data exists
   // draw the new shape
   objekt->old = objekt->current;
   GpiSetColor(hps,objekt->color);//objekt->color);
   switch (objekt->draw_type)
    {
    case DT_POLY :  GpiMove(hps,&(objekt->real[0]));
    							  GpiPolyLine (hps, objekt->nbr_points,objekt->real);
                    break;
    case DT_LINE :  for (count = 0; count < objekt->nbr_points/2; count++)
                      {
                      GpiMove(hps,&(objekt->real[count*2]));
                      GpiPolyLine(hps,2,&(objekt->real[count*2]));
                      }
        					  break;
    case DT_PIXEL:  GpiSetPel (hps, &(objekt -> current));
                    break;
    }
}

void translate_zoom_objekt (HPS hps, OBJEKT *objekt)
{
int count, x_rot, y_rot, x_temp;
POINTL *pShape;
pShape = objekt->shape;
   // remove old shape
   GpiSetColor(hps,CLR_BLACK);
   switch (objekt->draw_type)
       {
       case DT_POLY : GpiMove(hps,&(objekt->real[0]));
											GpiPolyLine (hps, objekt->nbr_points, objekt->real);                      break;
       case DT_LINE : for (count = 0; count < objekt->nbr_points/2; count++)
                        {
                        GpiMove(hps,&(objekt->real[count*2]));
                        GpiPolyLine(hps,2,&(objekt->real[count*2]));
                        }//GpiPolyLine (hps, 2, pReal);
       								break;
       case DT_PIXEL: GpiSetPel (hps, &(objekt -> old));
                      break;
       }
   // rotation
   if (objekt->draw_type != DT_PIXEL)              // pixel needs no rotation
      {
      if (objekt->angle < 0)
         objekt->angle += 360;;
      if (objekt->angle >= 360 )
         objekt->angle -= 360;
      for (count = 0; count < objekt->nbr_points; count++, pShape++)
          {
          x_rot =objekt->shape[count].x;
          y_rot =objekt->shape[count].y;
          x_temp = x_rot;
          x_rot = i_cos(x_rot,objekt->angle)-i_sin(y_rot,objekt->angle);
          y_rot = i_sin(x_temp,objekt->angle)+i_cos(y_rot,objekt->angle);
          objekt->real[count].x = (x_rot*objekt->zoom) + objekt->current.x;
          objekt->real[count].y = (y_rot*objekt->zoom) + objekt->current.y;
          }
      }
   // draw the new shape
   objekt->old = objekt->current;
   GpiSetColor(hps,objekt->color); //objekt->color);
   switch (objekt->draw_type)
       {
       case DT_POLY : GpiMove(hps,&(objekt->real[0]));
       								GpiPolyLine (hps, objekt->nbr_points, objekt->real);
                      break;
       case DT_LINE : for (count = 0; count < objekt->nbr_points/2; count++)
                        {
                        GpiMove(hps,&(objekt->real[count*2]));
                        GpiPolyLine(hps,2,&(objekt->real[count*2]));
                        }
       								break;
       case DT_PIXEL: GpiSetPel (hps, &(objekt -> current));
                      break;
       }
}

void move_objekt (OBJEKT *objekt)
{
if (objekt->thrust_speed == 0)
   return;
objekt->current.x += i_cos(objekt->thrust_speed,objekt->thrust_angle);
objekt->current.y += i_sin(objekt->thrust_speed,objekt->thrust_angle);
if (objekt->current.x > 2000)
   objekt->current.x -= 2000;
if (objekt->current.y > 2000)
   objekt->current.y -= 2000;
if (objekt->current.x <= 0)
   objekt->current.x += 2000;
if (objekt->current.y <= 0)
   objekt->current.y += 2000;
}

int collision (POINTL check_p, OBJEKT* objekt)
{
int count;
switch (objekt->ID)
  {
  case OT_MYSTSHIP:
          // 3 triangles
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[6]) == TRUE)
            return(TRUE);
          if (collision_trian(check_p,objekt->real[1],objekt->real[2],objekt->real[3]) == TRUE)
            return(TRUE);
          if (collision_trian(check_p,objekt->real[4],objekt->real[5],objekt->real[6]) == TRUE)
            return(TRUE);
          break;
  case OT_ENEMY2:
          // triangle
  case OT_ENEMY1:
          // triangle
          if (collision_trian(check_p,objekt->real[1],objekt->real[2],objekt->real[3]) == TRUE)
            return(TRUE);
          break;
  case OT_SHIELD:
          // simplified collision
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[3]) == TRUE)
            return(TRUE);
          break;
  case OT_REARSHOT:
          // 3 corners
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[5]) == TRUE)
            return(TRUE);
          break;
  case OT_LONGSHOT:
          // 3 corners
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[2]) == TRUE)
            return(TRUE);
          break;
  case OT_FASTSHOT:
          // 3 corners
          if (collision_trian(check_p,objekt->real[0], objekt->real[1],objekt->real[3]) == TRUE)
            return(TRUE);
          break;
  case OT_2SHOT:
          // 3 corners
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[3]) == TRUE)
            return(TRUE);
          break;
  case OT_SHIP:
          // collision with ship not 100% accurate but faster (ship is considered a true triangle)
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[3]) == TRUE)
            return(TRUE);
          break;
  case OT_SAT:
          // collision with sat
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[2]) == TRUE)
            return(TRUE);
          break;
  case OT_SMALLASTER:
          // check for 3 corners of asteroid (faster than next alg)
          if (collision_trian(check_p,objekt->real[0],objekt->real[1],objekt->real[2]) == TRUE)
            return(TRUE);
          break;
  default:
          // for all other types of asteroids
          for (count = 0; count < objekt->nbr_points-1; count++)
					  {
						// checks triangles based on real image (two corner points and center point)
            if (collision_trian(check_p, objekt->real[count], objekt->real[count+1], objekt->current) == TRUE)
              return (TRUE);        // collision
            }
          break;
       }
// no collision
return (FALSE);
}

void explode_asteroid (POINTL at, int ID)
{
int tmprnd, count;
switch (ID)
  {
  case OT_BIGASTER:
          if (random(2) == 0) // yep a powerup was found!
            {
            switch (random(7)) // number of powerups : 5 (shield 2x)
              {
              case 0: // super powerup fest
                      add_list(init_objekt(OT_LONGSHOT,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,long_shot,5),&start);
                      add_list(init_objekt(OT_REARSHOT,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,rear_shot,7),&start);
                      add_list(init_objekt(OT_SHIELD,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,shield,5),&start);
                      add_list(init_objekt(OT_FASTSHOT,at.x,at.y,random(360),random(4)+2,DT_LINE,355,CLR_PALEGRAY,fast_shot,12),&start);
                      add_list(init_objekt(OT_2SHOT,at.x,at.y,random(360),random(4)+2,DT_LINE,355, CLR_PALEGRAY,double_shot,10),&start);
                      break;
              case 1: // long shot
                      add_list(init_objekt(OT_LONGSHOT,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,long_shot,5),&start);
                      break;
              case 2: // rear shot
                      add_list(init_objekt(OT_REARSHOT,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,rear_shot,7),&start);
                      break;
              case 3:
              case 4: // shield
                      add_list(init_objekt(OT_SHIELD,at.x,at.y,random(360),random(4)+2,DT_POLY,355, CLR_PALEGRAY,shield,5),&start);
                      break;
              case 5: // fastshot
                      add_list(init_objekt(OT_FASTSHOT,at.x,at.y,random(360),random(4)+2,DT_LINE,355,CLR_PALEGRAY,fast_shot,12),&start);
                      break;
              case 6: // double shot
                      add_list(init_objekt(OT_2SHOT,at.x,at.y,random(360),random(4)+2,DT_LINE,355, CLR_PALEGRAY,double_shot,10),&start);
                      break;                              }
              }
            for (count = 0; count < 4; count++)
              {
              tmprnd = random(3)+1;
              switch (tmprnd)
                {
                case 1: // insert medium1asteroid
                        randomize_asteroids (mediumasteroid1, 8, 80);
                        add_list(init_objekt(OT_MEDIUMASTER,at.x,at.y,random(360), random(8)+4,DT_POLY,0, CLR_GREEN,mediumasteroid1,8),&start);
                        nbr_asteroids++;
                        break;
                case 2:  // insert medium2asteroid
                        randomize_asteroids (mediumasteroid2, 6, 70);
                        add_list(init_objekt(OT_MEDIUMASTER,at.x,at.y,random(360), random(10)+6,DT_POLY,0, CLR_DARKGREEN,mediumasteroid2,6),&start);
                        nbr_asteroids++;
                        break;
                case 3:  // insert small asteroid
                        randomize_asteroids (smallasteroid, 4, 50);
                        add_list(init_objekt(OT_SMALLASTER,at.x,at.y,random(360),  random(20)+4,DT_POLY,0, CLR_BROWN,smallasteroid,4),&start);
                        nbr_asteroids++;
                        break;
                }
              }
          break;
       case OT_MEDIUMASTER:
          for (count = 0; count < random(3)+1; count++)
            {
            // insert small asteroid
            randomize_asteroids (smallasteroid, 4, 50);
            add_list(init_objekt(OT_SMALLASTER,at.x,at.y,random(360), random(16)+2,DT_POLY,0, CLR_YELLOW,smallasteroid,4),&start);
            nbr_asteroids++;
            }
          break;
  }
}


void insert_message ( int at_ver, int at_hor, char *format, ...)
{
int count;
char msg_buffer[50];
va_list argptr;
va_start(argptr, format);
vsprintf(msg_buffer,format,argptr);
va_end(argptr);

// calculate first at_hor(inzontal) of message
if (at_hor == 0) // center message
   at_hor = 1000 - ((strlen(msg_buffer)*30)/2);
for (count = 0; count < strlen(msg_buffer); count++,at_hor+=30)
  {
  switch (tolower(msg_buffer[count]))
    {
    case 'a' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_A,sizeof(Logo_A)/sizeof(POINTL)),&effekts);
              break;
    case 'b' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_B,sizeof(Logo_B)/sizeof(POINTL)),&effekts);
              break;
    case 'c' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_C,sizeof(Logo_C)/sizeof(POINTL)),&effekts);
              break;
    case 'd' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_D,sizeof(Logo_D)/sizeof(POINTL)),&effekts);
              break;
    case 'e' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_E,sizeof(Logo_E)/sizeof(POINTL)),&effekts);
              break;
    case 'f' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_F,sizeof(Logo_F)/sizeof(POINTL)),&effekts);
              break;
    case 'g' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_G,sizeof(Logo_G)/sizeof(POINTL)),&effekts);
              break;
    case 'h' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_H,sizeof(Logo_H)/sizeof(POINTL)),&effekts);
              break;
    case 'i' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_I,sizeof(Logo_I)/sizeof(POINTL)),&effekts);
              break;
    case 'j' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_J,sizeof(Logo_J)/sizeof(POINTL)),&effekts);
              break;
    case 'k' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_K,sizeof(Logo_K)/sizeof(POINTL)),&effekts);
              break;
    case 'l' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_L,sizeof(Logo_L)/sizeof(POINTL)),&effekts);
              break;
    case 'm' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_M,sizeof(Logo_M)/sizeof(POINTL)),&effekts);
              break;
    case 'n' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_N,sizeof(Logo_N)/sizeof(POINTL)),&effekts);
              break;
    case 'o' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_O,sizeof(Logo_O)/sizeof(POINTL)),&effekts);
              break;
    case 'p' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_P,sizeof(Logo_P)/sizeof(POINTL)),&effekts);
              break;
    case 'q' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_Q,sizeof(Logo_Q)/sizeof(POINTL)),&effekts);
              break;
    case 'r' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_R,sizeof(Logo_R)/sizeof(POINTL)),&effekts);
              break;
    case 's' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_S,sizeof(Logo_S)/sizeof(POINTL)),&effekts);
              break;
    case 't' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_T,sizeof(Logo_T)/sizeof(POINTL)),&effekts);
              break;
    case 'u' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_U,sizeof(Logo_U)/sizeof(POINTL)),&effekts);
              break;
    case 'v' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_V,sizeof(Logo_V)/sizeof(POINTL)),&effekts);
              break;
    case 'w' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_W,sizeof(Logo_W)/sizeof(POINTL)),&effekts);
              break;
    case 'x' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_X,sizeof(Logo_X)/sizeof(POINTL)),&effekts);
              break;
    case 'y' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_Y,sizeof(Logo_Y)/sizeof(POINTL)),&effekts);
              break;
    case 'z' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_Z,sizeof(Logo_Z)/sizeof(POINTL)),&effekts);
              break;
    case '/' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_DEV,sizeof(Logo_DEV)/sizeof(POINTL)),&effekts);
              break;
    // numbers
    case '0' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_0,sizeof(Logo_0)/sizeof(POINTL)),&effekts);
              break;
    case '1' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_1,sizeof(Logo_1)/sizeof(POINTL)),&effekts);
              break;
    case '2' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_2,sizeof(Logo_2)/sizeof(POINTL)),&effekts);
              break;
    case '3' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_3,sizeof(Logo_3)/sizeof(POINTL)),&effekts);
              break;
    case '4' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_4,sizeof(Logo_4)/sizeof(POINTL)),&effekts);
              break;
    case '5' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_5,sizeof(Logo_5)/sizeof(POINTL)),&effekts);
              break;
    case '6' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_6,sizeof(Logo_6)/sizeof(POINTL)),&effekts);
              break;
    case '7' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_7,sizeof(Logo_7)/sizeof(POINTL)),&effekts);
              break;
    case '8' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_8,sizeof(Logo_8)/sizeof(POINTL)),&effekts);
              break;
    case '9' :add_list(init_objekt(OT_LOGO,at_hor,at_ver,0,0,DT_POLY,80,CLR_YELLOW,Logo_9,sizeof(Logo_9)/sizeof(POINTL)),&effekts);
              break;
    }
  }
}



// gonio stuff

void generate_tables (void)
{
double angle, tmp;
int tel;
for (tel = 0, angle = 0; tel < 360; tel++)
    {
    tmp = cos(angle) * GONIO_PRECISION;
    costable[tel] = (int) tmp;
    tmp = sin(angle) * GONIO_PRECISION;
    sintable[tel] = (int) tmp;
    angle += M_PI/180;
    }
}

int i_cos (int x, int angle)
{
long value;
while (angle >= 360)
      angle-=360;
while (angle < 0)
      angle+=360;
value = (long) costable[angle] * x;
value = value >> cos_shift;
return ((int) value);
}

int i_sin (int x, int angle)
{
long value;
while (angle >= 360)
      angle-=360;
while (angle < 0)
      angle+=360;
value = (long) sintable[angle] * x;
value = value >> sin_shift;
return ((int) value);
}

int to_deg (float rads)
{
int degrees;
degrees = (rads/(M_PI*2))*360;
return(degrees);
}

float to_rad (int degrees)
{
float ret_val;
ret_val = (M_PI*2/360)*degrees;
return (ret_val);
}

void objekt_seek (OBJEKT *objekt, int x, int y, int turnratio)
{
int ox, oy, targetangle;
// this routine enables enemy ships to try to seek the player ship
ox = objekt->current.x;
oy = objekt->current.y;
if (ox < x)
  {
  if (oy < y)
    targetangle = 45;
  else
    if (oy > y)
      targetangle = 315;
    else
      targetangle = 0;
  }
else
  {
  if (ox > x)
    {
    if (oy < y)
      targetangle = 135;
    else
      if (oy > y)
        targetangle = 225;
      else
        targetangle = 180;
    }
    else
      if (oy < y)
        targetangle = 90;
      else
        targetangle = 270;
  }
// ok we got the targetangle
// now ship must turn to target angle
if (abs(targetangle - objekt-> angle) >= 180) // turn the other way
  {
  if (targetangle > objekt->angle)
    objekt->angle -= turnratio;
  else
    if (targetangle < objekt->angle)
      objekt->angle += turnratio;
  }
else
  {
  if (targetangle > objekt->angle)
    objekt->angle += turnratio;
  else
    if (targetangle < objekt->angle)
      objekt->angle -= turnratio;
  }
}


# -*- coding: utf-8 -*-

#!/usr/bin/env python
import sys
sys.path.insert(1, sys.argv[1])
from duels import Subscriber
game = Subscriber()
init_msg = game.get_init()
print(" Abdellah Koutit / Flavien Midoux " )


class dict_to_obj(object):
    def __init__(self, adict):
        self.__dict__.update(adict)
def to_list(Pose):
    return [Pose.x/10,Pose.y/10]
def convert_vector(l):
    v=[]
    for i in range(len(l)):
        
        v+=[to_list(l[i])]
    return v
    
    
def to_object(msg):
    if isinstance(msg, list):
        return [to_object(v) for v in msg]
    elif isinstance(msg, dict):
        for v in msg:
            msg[v] = to_object(msg[v])
        return dict_to_obj(msg)
    return msg 
msg = to_object({"snake1":{"x":200,"y":240,"body":[[180,240],[160,240],[140,240],[120,240],[100,240],[80,240],[60,240],[40,240],[20,240],[20,260],[20,280]]},"snake2":{"x":240,"y":200,"body":[[240,180],[240,160],[260,160],[280,160],[300,160],[320,160],[320,140]]},"apples":{"x":240,"y":240}})


#print([[0,0]]+msg.snake1.body)
# add other imports here /(e.g. pygame) 

# Importations:
import pygame
from pygame.locals import *
import os
import pickle
import random
import sys
from pygame.font import FontType

# Déclaration des couleurs:
GREY = (255, 77, 45)
ORANGE = (237, 127, 16)
ROUGE = (255, 0, 0)

BLUE = (204, 204, 255)
VERT = (52, 201, 36)
WHITE = (200, 200, 200)
YELLOW = (255, 255, 0)
BLACK = (0,0,0)
w=15
fontSize=4*w
height=60*w
width=60*w
d=75
b=(-15)

def drawGrid():
    blockSize = 2 #Set the size of the grid block
    for x in range(w*0+d, w*50+d, w*blockSize):
        for y in range(w*10+d, w*50+d, w*blockSize):
            rect = pygame.Rect(x, y, w*blockSize, w*blockSize)
            pygame.draw.rect(screen, BLUE, rect, 1)
def draw():
    """Fonction dessin (mise en mémoire tampon)."""
    global food,  snake1_positions, snake2_positions
    this_font = pygame.font.SysFont(None, fontSize)
    # On efface l'écran:
    screen.fill(WHITE)
    
    # On colle la surface de l'image cartoon du serpent dans la zone de score pour la décorer:
    screen.blit(cartoon_snake, (w*24+d, w*1+d+b))

    # Si le serpent mange:
    
    drawGrid()
    pygame.draw.rect(screen, (ORANGE), Rect(w*6+d, 6+d+b, w*1, w*2), 0)
    pygame.draw.ellipse(screen, (ORANGE), Rect(w*6+d, 6+d+b, w*2, w*2), 0)
    
    
    pygame.draw.circle(screen , WHITE , (w*6+d+17,6+d+b+20) , 4)
    pygame.draw.circle(screen , WHITE , (w*6+d+17,6+d+b+10) , 4)
    pygame.draw.circle(screen , BLACK , (w*6+d+17,6+d+b+20) , 2)
    pygame.draw.circle(screen , BLACK , (w*6+d+17,6+d+b+10) , 2)
    
    
    pygame.draw.rect(screen, (ORANGE), Rect(w*4+d, 6+d+b, w*2, w*2), 0)

    pygame.draw.circle(screen, WHITE, (w*4+d+2*w,6+d+b),4)
    pygame.draw.circle(screen, WHITE, (w*4+d+2*w,6+d+b+2*w),4)


    pygame.draw.rect(screen, (ORANGE), Rect(w*2+d, 6+d+b, w*2, w*2), 0)
    pygame.draw.circle(screen, WHITE, (w*2+d+2*w,6+d+b),4)
    pygame.draw.circle(screen, WHITE, (w*2+d+2*w,6+d+b+2*w),4)
    pygame.draw.rect(screen, (ORANGE), Rect(w*0+d, 6+d+b, w*2, w*2), 0)
    pygame.draw.circle(screen, WHITE, (w*0+d+2*w,6+d+b),4)
    pygame.draw.circle(screen, WHITE, (w*0+d+2*w,6+d+b+2*w),4)
    
    pygame.draw.circle(screen, WHITE, (w*4+d-4*w,6+d+b),4)
    pygame.draw.circle(screen, WHITE, (w*4+d-4*w,6+d+b+2*w),4)
    
    pygame.draw.circle(screen, BLACK , (w*4+d+w, 6+d+b+w),4)
    pygame.draw.circle(screen, BLACK , (w*2+d+w, 6+d+b+w),4)
    pygame.draw.circle(screen, BLACK , (w*0+d+w, 6+d+b+w),4)
    score1_surface = this_font.render(str(score1), True, (ORANGE))
    
    screen.blit(score1_surface, (w*10+d, w*0+d+b))
    screen.blit(apple,(w*15+d,w*0+d+b))
    pygame.draw.rect(screen, (VERT), Rect(w*6+d, w*7+d+b, w*1, w*2), 0)
    pygame.draw.ellipse(screen, (VERT), Rect(w*6+d, w*7+d+b, w*2, w*2), 0)
    

    pygame.draw.circle(screen , BLACK , (w*6+d+17,w*7+d+b+20) , 4)
    pygame.draw.circle(screen , BLACK , (w*6+d+17,w*7+d+b+10) , 4)
    pygame.draw.circle(screen , WHITE , (w*6+d+17,w*7+d+b+20) , 3)
    pygame.draw.circle(screen , WHITE , (w*6+d+17,w*7+d+b+10) , 3)
    pygame.draw.ellipse(screen, (VERT), Rect(w*4+d, w*7+d+b, w*2, w*2), 0)
    pygame.draw.ellipse(screen, (VERT), Rect(w*2+d, w*7+d+b, w*2, w*2), 0)
    pygame.draw.ellipse(screen, (VERT), Rect(w*0+d, w*7+d+b, w*2, w*2), 0)
    
    score2_surface = this_font.render(str(score2), True, (VERT))
    screen.blit(score2_surface, (w*10+d, w*7+d+b))
    screen.blit(apple,(w*15+d,w*7+d+b))
    bot1_surface = this_font.render(init_msg.name1, True, (ORANGE))
    
    screen.blit(bot1_surface, (w*40+d, w*0+d+b))
    bot2_surface = this_font.render(init_msg.name2, True, (VERT))
    
    screen.blit(bot2_surface, (w*40+d, w*7+d+b))


    # On dessine une surface de jeu:
    
    pygame.draw.rect(screen, (GREY), Rect(w*0+d, w*10+d, w*50, w*40), 3)
    for index, item in enumerate(food_positions):
        screen.blit(apple,(w*item[0]+d, w*item[1]+d))    
    
    # On dessine les éléments du serpent:
    for index, item in enumerate(snake1_positions):
        # On dessine la tête en orange (contours uniquement):
        if index == 0:
            #pygame.draw.ellipse(screen,WHITE,pygame.Rect(x-14,y-23,16,20))
            pygame.draw.ellipse(screen, (ORANGE), Rect(w*item[0]+d, w*item[1]+d, w*2, w*2), 0)

            if msg.snake1.head.t == 0:  #right
                pygame.draw.rect(screen, (ORANGE), Rect(w*item[0]+d, w*item[1]+d, w*1, w*2), 0)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+20) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+10) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+20) , 2)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+10) , 2)
            if msg.snake1.head.t == 1:  #up
                pygame.draw.rect(screen, (ORANGE), Rect(w*item[0]+d, w*item[1]+w*1+d, w*2, w*1), 0)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+20,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+10,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+20,w*item[1]+d+w) , 2)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+10,w*item[1]+d+w) , 2)
            if msg.snake1.head.t == 2:  #left
                pygame.draw.rect(screen, (ORANGE), Rect(w*item[0]+w*1+d, w*item[1]+d, w*1, w*2), 0)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+20) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+10) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+20) , 2)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+10) , 2)
            if msg.snake1.head.t == 3:  #down
                pygame.draw.rect(screen, (ORANGE), Rect(w*item[0]+d, w*item[1]+d, w*2, w*1), 0)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+20,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+10,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+20,w*item[1]+d+w) , 2)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+10,w*item[1]+d+w) , 2)
        # On dessine le reste du corps en vert (contours uniquement):
        else:
            pygame.draw.rect(screen, (ORANGE), Rect(w*item[0]+d, w*item[1]+d, w*2, w*2), 15)
            pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+w) , 4)
            pygame.draw.circle(screen , WHITE , (w*item[0]+d,w*item[1]+d) , 4)
            pygame.draw.circle(screen , WHITE , (w*item[0]+d+2*w,w*item[1]+d+2*w) , 4)
            pygame.draw.circle(screen , WHITE , (w*item[0]+d+2*w,w*item[1]+d) , 4)
            pygame.draw.circle(screen , WHITE , (w*item[0]+d,w*item[1]+d+2*w) , 4)
            
    for index, item in enumerate(snake2_positions):
        # On dessine la tête en orange (contours uniquement):
        if index == 0:
            pygame.draw.ellipse(screen, (VERT), Rect(w*item[0]+d, w*item[1]+d, w*2, w*2), 0)
            if msg.snake2.head.t == 0:
                pygame.draw.rect(screen, (VERT), Rect(w*item[0]+d, w*item[1]+d, w*1, w*2), 0)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+20) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+10) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+20) , 3)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+10) , 3)
            if msg.snake2.head.t == 1:
                pygame.draw.rect(screen, (VERT), Rect(w*item[0]+d, w*item[1]+w*1+d, w*2, w*1), 0)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+20,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+10,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+20,w*item[1]+d+w) , 3)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+10,w*item[1]+d+w) , 3)
            if msg.snake2.head.t == 2:
                pygame.draw.rect(screen, (VERT), Rect(w*item[0]+w*1+d, w*item[1]+d, w*1, w*2), 0)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+20) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+w,w*item[1]+d+10) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+20) , 3)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+w,w*item[1]+d+10) , 3)
            if msg.snake2.head.t == 3:
                pygame.draw.rect(screen, (VERT), Rect(w*item[0]+d, w*item[1]+d, w*2, w*1), 0)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+20,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , BLACK , (w*item[0]+d+10,w*item[1]+d+w) , 4)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+20,w*item[1]+d+w) , 3)
                pygame.draw.circle(screen , WHITE , (w*item[0]+d+10,w*item[1]+d+w) , 3)
        # On dessine le reste du corps en vert (contours uniquement):
        else:
            pygame.draw.ellipse(screen, (VERT), Rect(w*item[0]+d, w*item[1]+d, w*2, w*2), 0)

    
    
    
    



def main_loop():
    while program:
        # Gestion des évènements:
        #event_loop()
        # Mise à jour:
        #update()
        # Dessin (mise en mémoire tampon):
        
        draw()
        # Affichage:
        pygame.display.flip()
        # Cadencage à 10 FPS:
        # timer.tick(10)
        
game.ready()
# INITIALISATION DE PYGAME  #
pygame.init()               #
#############################
again = True

while again:
    
    
    

    msg = game.refresh()
    if game.winner:
        break
    
    



    # FENETRE #####################################
    # Création d'une fenêtre:                     #
    screen = pygame.display.set_mode((height, width),pygame.RESIZABLE)  #
    # Modification du nom de la fenêtre:          #
    pygame.display.set_caption("Snake") #
    
    ###############################################



   
                                      #
                                                                                                               #
    # Création d'une variable score initialisée à zéro:                                                        #
    score1 = msg.score1   
    score2 = msg.score2                                                                                          #
    # Chargement d'une font:                                                                                   #
    this_font = pygame.font.SysFont(None, 40)                                                 #
                                                                        #
                                                                                                               #
    # Création d'une surface pour l'image cartoon du serpent: 
    game_path=os.path.abspath(os.path.dirname(__file__))
    def get_image(filename):
            return pygame.image.load('{}/sprites/{}'.format(game_path, filename))                                                 #
    cartoon_snake = get_image("cartoon_snake.png").convert_alpha()  
    cartoon_snake = pygame.transform.scale(cartoon_snake,(w*8,w*8))
    apple =   get_image("apple.png").convert_alpha()     
    apple=pygame.transform.scale(apple,(w*2,w*2))    
      
                      #
    # Collage de la surface de l'image cartoon du serpent dans la zone de score pour la décorer:               #
    screen.blit(cartoon_snake, (w*24+d, w*1+d))                                                                      #
    pygame.display.set_icon(cartoon_snake)                                                                                                         #
                                         #
                                                                                                               #
                                                                      #
    ############################################################################################################

                          #
                                                                                                                   #
    # Dessin du serpent et mise en variable de sa position:                                                        #
    snake1 = pygame.draw.rect(screen, (ORANGE), Rect(0+d, 24+d, 2, 2), 2)  
    snake2 = pygame.draw.rect(screen, (BLUE), Rect(0+d, 24+d, 2, 2), 2) 
                                              #
    # snake1_positions = [[0, 240],[20,240],[40,240],[60,240]] 
    # snake2_positions = [[120, 240],[140,240],[160,240]]         
    
    snake1_positions = [to_list(msg.snake1.head)]+convert_vector(msg.snake1.body)
    snake2_positions = [to_list(msg.snake2.head)]+convert_vector(msg.snake2.body)                                                                          #
                                                                                                                   #
                                          #
    food_positions = convert_vector(msg.apples)                                                                                     #
                                                                                                                   #
    # Création d'une surface pour le game over:                                                                    #
    game_over_surface = this_font.render("G A M E    O V E R!", True, (ROUGE))                               #
    # Création d'un rectangle pour la surface game over (dans un but futur (lors de l'affichage) de centrage):     #
    game_over_surface_rect = game_over_surface.get_rect(midtop=(w*24+d, w*24+d))                                         #
                                                                                                                   #
                                                                                                                   #
    # Création d'une surface pour rejouer:                                                                         #
    rejouer_surface = this_font.render("PLAY AGAIN", True, (ORANGE))                                         #
    # Création d'un rectangle pour la surface rejouer (dans un but futur (lors de l'affichage) de centrage):       #
    rejouer_surface_rect = rejouer_surface.get_rect(midtop=(w*24+d, w*28+d))                                             #
    ################################################################################################################



    
    draw()
    pygame.display.flip()

    
this_font = pygame.font.SysFont(None, w*4) 
# FENETRE #####################################
# Création d'une fenêtre:                     #
screen = pygame.display.set_mode((height, width),pygame.RESIZABLE)  #
# Modification du nom de la fenêtre:          #
pygame.display.set_caption("AK::Snake") #

###############################################


# Création d'une surface pour le game over:                                                                    #
game_over_surface = this_font.render("G A M E    O V E R!", True, (ROUGE))                               #
# Création d'un rectangle pour la surface game over (dans un but futur (lors de l'affichage) de centrage):     #
game_over_surface_rect = game_over_surface.get_rect(midtop=(w*24+d, w*24+d))                                         #
                                                                                                           #
                                                                                                           #
# Création d'une surface pour rejouer:                                                                         #
rejouer_surface = this_font.render("PLAY AGAIN", True, (ORANGE))                                         #
# Création d'un rectangle pour la surface rejouer (dans un but futur (lors de l'affichage) de centrage):       #
rejouer_surface_rect = rejouer_surface.get_rect(midtop=(w*16+d, w*32+d)) 





# On dessine une surface de jeu:
pygame.draw.rect(screen, (GREY), Rect(w*0, w*10, w*50, w*40), 1)
def game_over():
	draw()
	game_over_surface = this_font.render("G A M E    O V E R!", True, (GREY)) 
	rejouer_surface = this_font.render(text, True, (GREY)) 

	
	screen.blit(game_over_surface, game_over_surface_rect)
	screen.blit(rejouer_surface, rejouer_surface_rect)
	pygame.display.flip()
	


if game.winner == 3:
    text = 'Draw'
    game_over()

elif game.winner < 0:
    text = 'A bug stopped the game'
    game_over()
else: 
    text =  game.winner_name(init_msg)+ ' WIN THE GAME'
   
    game_over()


pygame.display.update()

t0 = pygame.time.get_ticks()
while pygame.time.get_ticks() - t0 < 5000:
    if any(event.type == pygame.QUIT for event in pygame.event.get()):
        break

pygame.quit()


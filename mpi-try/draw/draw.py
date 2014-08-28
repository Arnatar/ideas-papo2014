from pprint import pprint
import pygame, sys
from pygame.locals import *
from pygame import draw

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

SIZE=10
AMOUNT=50
class Draw():
    def __init__(self, data):

        self.data = data


        # pygame.display.flip()


        self.init_pygame()
        self.run()

        # pygame.display.update()


    def init_pygame(self):
        pygame.init()
        pygame.display.set_caption('Hello world!')
        self.screen = pygame.display.set_mode((AMOUNT*SIZE,AMOUNT*SIZE), 0, 32)
        self.clock = pygame.time.Clock()

    def draw_point(self,x,y):
        draw.rect(self.screen, BLACK, [x, y, SIZE, SIZE])

    def print_step(self, nr):
        step = self.data[nr]
        print step
        print(self.data[nr] == self.data[nr-1])
        for y, row in enumerate(step):
            for x, col in enumerate(row):
                if not col == [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y)


    def run(self):
        done=False
        tic=0
        step=0
        while not done:
            for event in pygame.event.get():
                if event.type == QUIT:
                    done = True

            self.screen.fill(WHITE)

            # pygame.display.flip()
            # self.clock.tick(1)
            # tic+=1
            # if tic == 100:
            #     step+=1
            #     tic=0

            self.print_step(step)



            # if tic==100:
                # x+=5
                # tic=0

            # pygame.draw.line(self.screen, GREEN, [0, 0], [50,30], 5)
            # pygame.draw.rect(self.screen, WHITE, [150, 10, 50, 20])

            # self.draw_point(10+x,10)

            pygame.display.flip()
            self.clock.tick(10)

            step+=1
            if step > len(self.data)-2: done = True

        pygame.quit()





from pprint import pprint
import pygame, sys
from pygame.locals import *
from pygame import draw

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

# size of one piece
SIZE=10
# amount of pieces x/y
AMOUNT=50
class Draw():
    def __init__(self, data):

        self.data = data


        # pygame.display.flip()


        self.init_pygame()
        self.doubles = {}
        self.run()

        # pygame.display.update()


    def init_pygame(self):
        pygame.init()
        pygame.display.set_caption('Ideas')
        self.screen = pygame.display.set_mode((AMOUNT*SIZE,AMOUNT*SIZE), 0, 32)
        self.clock = pygame.time.Clock()

    def draw_point(self,x,y, qual):
        color=25*qual
        draw.rect(self.screen, (color,color,color), [x, y, SIZE, SIZE])

    def print_step(self, nr):
        step = self.data[nr]
        # pprint(step)
        # print(self.data[nr] == self.data[nr-1])
        for y, row in enumerate(step):
            for x, col in enumerate(row):
                if not col == [0,0,0,0]:
                    if nr>0 and self.data[nr-1][y][x] == col:
                        if (y,x,tuple(col)) in self.doubles:
                            self.doubles[(y,x,tuple(col))] += 1

                        else:
                            self.doubles[(y,x,tuple(col))] = 0

                    # print(y,x)
                    self.draw_point(SIZE*x,SIZE*y, col[0])


    def run(self):
        done=False
        tic=0
        step=0
        while not done:
            for event in pygame.event.get():
                if event.type == QUIT:
                    done = True

            self.screen.fill(BLACK)

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
            self.clock.tick(30)

            step+=1
            if step > len(self.data)-2: done = True


        pprint([(k,v) for k,v in self.doubles.items() if v>5])
        pygame.quit()





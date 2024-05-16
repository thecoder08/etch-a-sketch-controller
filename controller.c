#include <xgfx/window.h>
#include <xgfx/drawing.h>
#include "lerp.h"
#include <stdio.h>
#include <stdlib.h>

Point finalPath[1000];

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Missing resolution argument\n");
        return 1;
    }
    float resolution = strtof(argv[1], NULL);
    initWindow(640, 480, "Etch-a-sketch Controller");
    int wholeT = 0;
    float partT = 0;
    printf("0 0\n");
    while(1) {
        Event event;
        while (checkWindowEvent(&event)) {
            if (event.type == WINDOW_CLOSE) {
                return 0;
            }
        }
        int pointIndex = 0;
        Point a = {
            .x = 300,
            .y = 100
        };
        Point b = {
            .x = 500,
            .y = 30
        };
        Point c = {
            .x = 300,
            .y = 300
        };
        Point d = {
            .x = 100,
            .y = 30
        };
        Point old = a;
        clear();
        line(0, 0, a.x, 0, 0xff00ff00);
        line(a.x, 0, a.x, a.y, 0xff00ff00);
        finalPath[0].x = 0;
        finalPath[0].y = 0;
        finalPath[1].x = a.x;
        finalPath[1].y = a.y;
        pointIndex = 2;
        for (float t = resolution; t < 1.0001; t += resolution) {
            Point new = quadBezier(a, b, c, t);
            line(old.x, old.y, new.x, old.y, 0xff0000ff);
            line(new.x, old.y, new.x, new.y, 0xff0000ff);
            old = new;
            finalPath[pointIndex] = new;
            pointIndex++;
        }
        for (float t = resolution; t < 1.0001; t += resolution) {
            Point new = quadBezier(c, d, a, t);
            line(old.x, old.y, new.x, old.y, 0xff0000ff);
            line(new.x, old.y, new.x, new.y, 0xff0000ff);
            old = new;
            finalPath[pointIndex] = new;
            pointIndex++;
        }
        line(a.x, a.y, 0, a.y, 0xffff0000);
        line(0, a.y, 0, 0, 0xffff0000);
        Point partPoint = {
            .x = finalPath[wholeT + 1].x,
            .y = finalPath[wholeT].y
        };
        partT += 0.05;
        Point current;
        if (partT < 0.5) {
            current = lerp(finalPath[wholeT], partPoint, partT*2);
        }
        else {
            current = lerp(partPoint, finalPath[wholeT + 1], partT*2-1);
        }
        circle(current.x, current.y, 5, 0xffffffff);
        if (partT >= 1) {
            partT = 0;
            updateWindow(); // hacky
            wholeT++;
            //getchar();
            printf("%d %d\n", finalPath[wholeT].x, finalPath[wholeT].y);
        }
        if (wholeT >= pointIndex) {
            wholeT = 0;
        }
        updateWindow();
    }
}
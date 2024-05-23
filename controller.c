#include <xgfx/window.h>
#include <xgfx/drawing.h>
#include "lerp.h"
#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

Point finalPath[1000];

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "You must specify resolution and serial port\n");
        return 1;
    }
    int serial = open(argv[2], O_RDWR | O_NOCTTY | O_SYNC);
    if (serial < 0) {
        fprintf(stderr, "Error connecting to serial port: %s\n", strerror(errno));
        return 1;
    }
    set_interface_attribs(serial, B9600, 0);
    set_blocking(serial, 0);
    sleep(2);

    float resolution = strtof(argv[1], NULL);
    initWindow(640, 480, "Etch-a-sketch Controller");
    int wholeT = 0;
    float partT = 0;

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
        Point current;
        if (partT < 0.5) {
            current = lerp(finalPath[wholeT], partPoint, partT*2);
        }
        else {
            current = lerp(partPoint, finalPath[wholeT + 1], partT*2-1);
        }
        circle(current.x, current.y, 5, 0xffffffff);
        if (partT == 0) {
            char pointbuf[100];
            int length = sprintf(pointbuf, "%d %d\n", finalPath[wholeT].x * 10, finalPath[wholeT].y * 10);
            puts(pointbuf);
            write(serial, pointbuf, length);
            getchar();
        }
        partT += 0.05;
        if (partT >= 1) {
            partT = 0;
            wholeT++;
        }
        if (wholeT >= pointIndex) {
            wholeT = 0;
        }
        updateWindow();
    }
}
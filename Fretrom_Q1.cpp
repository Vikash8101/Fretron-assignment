#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <algorithm> // for std::max and std::min

struct Point {
    int x, y;
    Point(int a, int b): x(a), y(b) {}
};

bool onSegment(Point p, Point q, Point r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}

int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  
    return (val > 0) ? 1 : 2;
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; 
}

void drawFlightPath(SDL_Renderer* renderer, const std::vector<Point>& flightPath) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (size_t i = 0; i < flightPath.size() - 1; ++i) {
        SDL_RenderDrawLine(renderer, flightPath[i].x, flightPath[i].y, flightPath[i + 1].x, flightPath[i + 1].y);
    }
}

bool checkPathsIntersect(const std::vector<Point>& path1, const std::vector<Point>& path2) {
    for (size_t i = 0; i < path1.size() - 1; ++i) {
        for (size_t j = 0; j < path2.size() - 1; ++j) {
            if (doIntersect(path1[i], path1[i + 1], path2[j], path2[j + 1])) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    std::vector<Point> flight1 = {Point(11, 2), Point(2, 2), Point(33, 33)};
    std::vector<Point> flight2 = {Point(11, 2), Point(2, 4), Point(32, 32)};
    std::vector<Point> flight3 = {Point(11, 42), Point(42, 34)};

    bool intersect12 = checkPathsIntersect(flight1, flight2);
    bool intersect13 = checkPathsIntersect(flight1, flight3);
    bool intersect23 = checkPathsIntersect(flight2, flight3);

    if (intersect12 || intersect13 || intersect23) {
        std::cout << "Flight paths intersect. Safety violation!" << std::endl;
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Flight Paths", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawFlightPath(renderer, flight1);
    drawFlightPath(renderer, flight2);
    drawFlightPath(renderer, flight3);

    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

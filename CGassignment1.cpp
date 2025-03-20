#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

// 화면 해상도
const int WIDTH = 512;
const int HEIGHT = 512;

// 벡터 연산을 위한 구조체
struct Vec3 {
    float x, y, z;
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / len, y / len, z / len);
    }
};

// 광선 구조체
struct Ray {
    Vec3 origin, direction;
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction.normalize()) {}
};

// 구 객체
struct Sphere {
    Vec3 center;
    float radius;
    Sphere(const Vec3& center, float radius) : center(center), radius(radius) {}

    bool intersect(const Ray& ray, float& t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;
        t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        return (t > 0);
    }
};

// 카메라 설정
struct Camera {
    Vec3 eye;
    float l, r, b, t, d;
    Camera(Vec3 eye, float l, float r, float b, float t, float d)
        : eye(eye), l(l), r(r), b(b), t(t), d(d) {
    }

    Ray generateRay(int x, int y) const {
        float u = l + (r - l) * (x + 0.5f) / WIDTH;
        float v = b + (t - b) * (y + 0.5f) / HEIGHT;
        Vec3 direction = Vec3(u, v, -d).normalize();
        return Ray(eye, direction);
    }
};

// 장면 설정
struct Scene {
    std::vector<Sphere> spheres;

    void addSphere(const Sphere& sphere) {
        spheres.push_back(sphere);
    }

    bool intersect(const Ray& ray, Vec3& color) const {
        float tMin = std::numeric_limits<float>::max();
        bool hitSphere = false;

        // 구와 교차 검사
        for (const auto& sphere : spheres) {
            float t;
            if (sphere.intersect(ray, t) && t < tMin) {
                tMin = t;
                hitSphere = true; // 구와 충돌했다면 플래그 설정
            }
        }

        // 색상 결정: 구와 충돌한 경우만 흰색
        if (hitSphere) {
            color = Vec3(255, 255, 255); // 구와 충돌하면 흰색
        }
        else {
            color = Vec3(0, 0, 0); // 나머지는 검정색
        }

        return hitSphere;
    }
};

// 이미지 데이터 저장
unsigned char image[HEIGHT][WIDTH][3];
Scene scene;

// 렌더링 함수
void renderScene() {
    Camera camera(Vec3(0, 0, 0), -0.1f, 0.1f, -0.1f, 0.1f, 0.1f);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Ray ray = camera.generateRay(x, y);
            Vec3 color;
            scene.intersect(ray, color);
            image[y][x][0] = static_cast<unsigned char>(color.x);
            image[y][x][1] = static_cast<unsigned char>(color.y);
            image[y][x][2] = static_cast<unsigned char>(color.z);
        }
    }
}

// OpenGL 디스플레이 콜백
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
    glFlush();
}

int main(int argc, char** argv) {
    scene.addSphere(Sphere(Vec3(0, 0, -7), 2)); // 중앙 구 하나만 추가

    renderScene();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Ray Tracer");
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

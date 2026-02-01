#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include <string>

constexpr int width  = 800;
constexpr int height = 800;
constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};
void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color)
{
    bool steep = abs(by - ay) > abs(bx - ax);
    if (steep) { // 直线陡峭时, 交换宽高(for遍历填充像素时, 永远沿x, y中上像素较多的那个方向上遍历), 避免线不连续
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) { // 永远从左向右遍历, 否则下面的for循环走不进去. 注意这两次if的顺序不能交换
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x = ax; x <= bx; ++x) {
        if (steep) { // 之前交换过宽高的话, 这里要还原
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }

        ierror += 2 * std::abs(by - ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }
}

std::pair<int, int> project(const Vec3& vertex)
{
    int x = static_cast<int>((vertex.x + 1.0f) * width / 2.0f);
    int y = static_cast<int>((vertex.y + 1.0f) * height / 2.0f);
    return std::make_pair(x, y);
}
int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model model;
    if (model.LoadModel("obj/diablo3_pose/diablo3_pose.obj")) {
        std::cout << "Load success! " << "vectexCount:" << model.vertices.size() << "  "<< "faceCount:" << model.faces.size() << std::endl;
    }
    if (!model.faces.empty()) { // for test
        std::cout << "First face vertex index:" << std::endl;
        int v = model.faces[0].vertices[0].x;
        int vt = model.faces[0].vertices[0].y;
        int vn = model.faces[0].vertices[0].z;

        std::cout << "v:[" << v << " " << model.vertices[v-1].x << " " << model.vertices[v-1].y << " " << model.vertices[v-1].z << "]" << std::endl;
        std::cout << "vt:[" << vt << " " << model.texCoords[vt-1].x << " " << model.texCoords[vt-1].y << " " << model.texCoords[vt-1].z << "]" <<std::endl;
        std::cout << "vn:[" << vn << " " << model.normals[vn-1].x << " " << model.normals[vn-1].y << " " << model.normals[vn-1].z << "]" << std::endl;
    }
    for (auto& face : model.faces) {
        const Vec3& va = model.vertices[face.vertices[0].x - 1];
        const Vec3& vb = model.vertices[face.vertices[1].x - 1];
        const Vec3& vc = model.vertices[face.vertices[2].x - 1];
        auto [ax, ay] = project(va);
        auto [bx, by] = project(vb);
        auto [cx, cy] = project(vc);
        line(ax, ay, bx, by, framebuffer, red);
        line(bx, by, cx, cy, framebuffer, red);
        line(cx, cy, ax, ay, framebuffer, red);
        framebuffer.set(ax, ay, white);
        framebuffer.set(bx, by, white);
        framebuffer.set(cx, cy, white);
    }

    for (auto& vertex : model.vertices) {
        auto [x, y] = project(vertex);
        framebuffer.set(x, y, white);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    
    return 0;
}




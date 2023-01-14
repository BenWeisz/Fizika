#pragma once

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>

// utility structure for realtime plot written by Evan Pezant
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

class EnergyPanel {
   private:
    ScrollingBuffer* mScrollingBufferT;
    ScrollingBuffer* mScrollingBufferV;
    ScrollingBuffer* mScrollingBufferTot;
    static float Time;

   public:
    EnergyPanel();
    ~EnergyPanel();
    void Draw();
    void AddPoint(const float T, const float V);
};
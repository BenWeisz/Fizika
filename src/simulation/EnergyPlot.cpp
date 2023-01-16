#include "simulation/EnergyPlot.hpp"

float EnergyPlot::Time = 0.0f;

EnergyPlot::EnergyPlot() {
    mScrollingBufferT = new ScrollingBuffer();
    mScrollingBufferV = new ScrollingBuffer();
    mScrollingBufferTot = new ScrollingBuffer();
}

EnergyPlot::~EnergyPlot() {
    delete mScrollingBufferT;
    delete mScrollingBufferV;
    delete mScrollingBufferTot;
}

void EnergyPlot::Draw() {
    Time += ImGui::GetIO().DeltaTime;

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    static ImPlotAxisFlags flags = ImPlotAxisFlags_None;
    if (ImPlot::BeginPlot("Energies", ImVec2(-1, -1))) {
        ImPlot::SetupAxes("time (s)", "energy", flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, Time - history, Time, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 30);
        ImPlot::PlotLine("Kinetic", &mScrollingBufferT->Data[0].x, &mScrollingBufferT->Data[0].y, mScrollingBufferT->Data.size(), 0, mScrollingBufferT->Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Potential", &mScrollingBufferV->Data[0].x, &mScrollingBufferV->Data[0].y, mScrollingBufferV->Data.size(), 0, mScrollingBufferV->Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Total", &mScrollingBufferTot->Data[0].x, &mScrollingBufferTot->Data[0].y, mScrollingBufferTot->Data.size(), 0, mScrollingBufferTot->Offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
}

void EnergyPlot::AddPoint(const float T, const float V) {
    // Add the kenetic energy, potential and total energy to the buffer
    mScrollingBufferT->AddPoint(Time, T);
    mScrollingBufferV->AddPoint(Time, V);
    mScrollingBufferTot->AddPoint(Time, (T + V));
}
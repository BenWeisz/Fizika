#pragma once

#include "DataBuildState.hpp"

#include "util/types.hpp"

namespace DataBuild {

class Pipeline {
    //    private:
    // static cl context goes here
   public:
    Pipeline() = delete;
    ~Pipeline();
    bool InitPipeline();
    // bool RunTransformOperator(const PipelineInputData* )
};

};  // namespace DataBuild
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    static glm::mat4 GeneratePipelineOperatorTransform(const std::vector<PipelineOperator>& pipelineOperators) {
        glm::mat4 transform(1.0f);
        for (u32 i = 0; i < pipelineOperators.size(); i++) {
            const PipelineOperator* pipelineOperator = &(pipelineOperators[pipelineOperators.size() - i - 1]);
            const float v0 = pipelineOperator->v0;
            const float v1 = pipelineOperator->v1;
            const float v2 = pipelineOperator->v2;

            if (pipelineOperator->mPipelineOperatorType == PipelineOperatorType::SCALE)
                transform = glm::scale(transform, glm::vec3(v0, v1, v2));
            else if (pipelineOperator->mPipelineOperatorType == PipelineOperatorType::TRANSLATE)
                transform = glm::translate(transform, glm::vec3(v0, v1, v2));
            else if (pipelineOperator->mPipelineOperatorType == PipelineOperatorType::ROTATE) {
                transform = glm::rotate(transform, glm::radians(v2), glm::vec3(1.0f, 0.0f, 0.0f));
                transform = glm::rotate(transform, glm::radians(v1), glm::vec3(0.0f, 0.0f, 1.0f));
                transform = glm::rotate(transform, glm::radians(v0), glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }

        return transform;
    }
};

};  // namespace DataBuild
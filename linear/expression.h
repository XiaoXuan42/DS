#pragma once

#include "stack.h"
#include "../string/string.h"

namespace DS
{
    class MiddleExpression
    {
        String exp;
    public:
        MiddleExpression(const String &exp): exp(exp) {}
        MiddleExpression() {}
        ~MiddleExpression();
    };
    class SuffixExpression
    {
        String exp;    
    public:
        SuffixExpression();
        SuffixExpression(const MiddleExpression &m_e);
        ~SuffixExpression();
    };
}
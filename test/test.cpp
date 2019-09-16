#include <iostream>
#include "../DS/linear/link.h"
#include "../DS/linear/stack.h"
#include <fstream>

int main() {
    DS::Stack<int> st;
    int n;
    std::cin >> n;
    for(int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        st.push(x);
    }
    while(!st.empty()) {
        printf("%d ", st.top());
        st.pop();
    }
    return 0;
}
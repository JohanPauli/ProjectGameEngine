#include "InputContext.h"

int InputContext::_nextId = 0;

// comparator function
inline bool inputContext_lessThan(InputContext* lhs, InputContext* rhs) { return lhs->getInputContextId() < rhs->getInputContextId(); }
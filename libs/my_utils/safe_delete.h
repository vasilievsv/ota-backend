#pragma once
#ifndef _X_UTILS_SAFE_DELETE_H
#define _X_UTILS_SAFE_DELETE_H

#define SAFE_DELETE(ptr)                \
              delete ptr;               \
              ptr = NULL;               \

#endif
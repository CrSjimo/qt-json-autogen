#ifndef CONSTRAINT_TEST_H
#define CONSTRAINT_TEST_H

#include "qjsonstream.h"

// Test struct with various constraints
class TestStruct {
public:
    // Numeric range constraints
    __qas_constraint__(MINIMUM 0 MAXIMUM 100)
    int score;
    
    // String length constraints
    __qas_constraint__(MIN_LENGTH 3 MAX_LENGTH 20)
    QString name;
    
    // Pattern constraint for email
    __qas_constraint__(PATTERN "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$")
    QString email;
    
    // Enum constraint for status
    __qas_constraint__(ENUM ["active", "inactive", "pending"])
    QString status;
    
    // Const constraint for version
    __qas_constraint__(CONST "1.0.0")
    QString version;
    
    // Multiple constraint groups (OR relationship)
    __qas_constraint__(MINIMUM 18)
    __qas_constraint__(CONST -1)  // Special case for unknown age
    int age;
    
    // Exclusive range constraints
    __qas_constraint__(EXCLUSIVE_MINIMUM 0.0 EXCLUSIVE_MAXIMUM 1.0)
    double probability;

    // No constraint
    int someValue;

};

QAS_JSON_NS(TestStruct)

#endif // CONSTRAINT_TEST_H
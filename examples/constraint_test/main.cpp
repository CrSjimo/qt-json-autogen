#include <QCoreApplication>
#include <QJsonDocument>
#include <QDebug>

#include "constraint_test.h"

void testValidConstraints() {
    qDebug() << "=== Testing Valid Constraints ===";
    
    // Create a valid test object
    TestStruct validObj;
    validObj.score = 85;  // Valid: 0 <= 85 <= 100
    validObj.name = "Alice";  // Valid: 3 <= 5 <= 20
    validObj.email = "alice@example.com";  // Valid: matches email pattern
    validObj.status = "active";  // Valid: in enum ["active", "inactive", "pending"]
    validObj.version = "1.0.0";  // Valid: matches const
    validObj.age = 25;  // Valid: >= 18
    validObj.probability = 0.75;  // Valid: 0.0 < 0.75 < 1.0
    validObj.someValue = 42;
    
    // Convert to JSON
    QJsonObject jsonObj = qAsClassToJson(validObj);
    qDebug() << "Valid object JSON:" << QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
    
    // Try to deserialize back
    TestStruct deserializedObj;
    QAS::JsonStream stream(jsonObj);
    stream >> deserializedObj;
    
    if (stream.good()) {
        qDebug() << "[OK] Valid constraints test passed";
    } else {
        qDebug() << "[FAIL] Valid constraints test failed with status:" << stream.status();
    }
}

void testInvalidConstraints() {
    qDebug() << "\n=== Testing Invalid Constraints ===";
    
    // Test various constraint violations
    QJsonObject invalidObjs[] = {
        // Score out of range
        QJsonObject{
            {"score", 150},  // Invalid: > 100
            {"name", "Alice"},
            {"email", "alice@example.com"},
            {"status", "active"},
            {"version", "1.0.0"},
            {"age", 25},
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Name too short
        QJsonObject{
            {"score", 85},
            {"name", "Al"},  // Invalid: length < 3
            {"email", "alice@example.com"},
            {"status", "active"},
            {"version", "1.0.0"},
            {"age", 25},
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Invalid email pattern
        QJsonObject{
            {"score", 85},
            {"name", "Alice"},
            {"email", "invalid-email"},  // Invalid: doesn't match email pattern
            {"status", "active"},
            {"version", "1.0.0"},
            {"age", 25},
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Invalid status enum
        QJsonObject{
            {"score", 85},
            {"name", "Alice"},
            {"email", "alice@example.com"},
            {"status", "unknown"},  // Invalid: not in enum
            {"version", "1.0.0"},
            {"age", 25},
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Wrong version constant
        QJsonObject{
            {"score", 85},
            {"name", "Alice"},
            {"email", "alice@example.com"},
            {"status", "active"},
            {"version", "2.0.0"},  // Invalid: doesn't match const "1.0.0"
            {"age", 25},
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Age too young (but not -1)
        QJsonObject{
            {"score", 85},
            {"name", "Alice"},
            {"email", "alice@example.com"},
            {"status", "active"},
            {"version", "1.0.0"},
            {"age", 16},  // Invalid: < 18 and != -1
            {"probability", 0.75},
            {"someValue", 42}
        },
        
        // Probability out of exclusive range
        QJsonObject{
            {"score", 85},
            {"name", "Alice"},
            {"email", "alice@example.com"},
            {"status", "active"},
            {"version", "1.0.0"},
            {"age", 25},
            {"probability", 1.0},  // Invalid: not < 1.0 (exclusive)
            {"someValue", 42}
        }
    };
    
    const char* testNames[] = {
        "Score out of range",
        "Name too short", 
        "Invalid email pattern",
        "Invalid status enum",
        "Wrong version constant",
        "Age too young",
        "Probability out of range"
    };
    
    for (int i = 0; i < 7; i++) {
        TestStruct testObj;
        QAS::JsonStream stream(invalidObjs[i]);
        stream >> testObj;
        
        if (stream.status() & QAS::JsonStream::ConstraintViolation) {
            qDebug() << "[OK]" << testNames[i] << "correctly failed constraint validation";
        } else {
            qDebug() << "[FAIL]" << testNames[i] << "should have failed but didn't, status:" << stream.status();
        }
    }
}

void testSpecialAgeCase() {
    qDebug() << "\n=== Testing Special Age Case (OR constraints) ===";
    
    // Test age = -1 (should be valid due to OR constraint)
    QJsonObject specialAgeObj{
        {"score", 85},
        {"name", "Alice"},
        {"email", "alice@example.com"},
        {"status", "active"},
        {"version", "1.0.0"},
        {"age", -1},  // Valid: matches CONST -1 constraint
        {"probability", 0.75},
        {"someValue", 42}
    };
    
    TestStruct testObj;
    QAS::JsonStream stream(specialAgeObj);
    stream >> testObj;
    
    if (stream.good()) {
        qDebug() << "[OK] Special age case (-1) correctly passed validation";
    } else {
        qDebug() << "[FAIL] Special age case (-1) should have passed, status:" << stream.status();
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    qDebug() << "Qt JSON Constraint Validation Test";
    qDebug() << "===================================";
    
    testValidConstraints();
    testInvalidConstraints();
    testSpecialAgeCase();
    
    qDebug() << "\nTest completed.";
    
    return 0;
}
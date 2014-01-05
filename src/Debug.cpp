#include "Debug.h"
#include <string>
using namespace std;

void gameDebugCallbackARB(GLenum source, GLenum type, GLuint id, 
                          GLenum severity, GLsizei length, 
                          const GLchar *message, GLvoid *userParam)
{

#ifdef _WIN32
    OutputDebugString("ARB DEBUG CALLBACK:\n");
#endif
    cout << "ARB DEBUG CALLBACK:" << endl;
    const char *sourceFmt = "UNDEFINED";
    switch(source)
    {
    case GL_DEBUG_SOURCE_API_ARB:             
        sourceFmt = "API"; 
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   
        sourceFmt = "WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: 
        sourceFmt = "SHADER_COMPILER"; 
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     
        sourceFmt = "THIRD_PARTY"; 
        break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:     
        sourceFmt = "APPLICATION"; 
        break;
    case GL_DEBUG_SOURCE_OTHER_ARB:           
        sourceFmt = "OTHER"; 
        break;
    }

#ifdef _WIN32
    OutputDebugString("Source: ");
    OutputDebugString(sourceFmt);
    OutputDebugString("\n");
#endif
    cout << "Source: " << sourceFmt << endl;

    const char *typeFmt = "UNDEFINED";
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:               
        typeFmt = "ERROR"; 
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: 
        typeFmt = "DEPRECATED_BEHAVIOR"; 
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  
        typeFmt = "UNDEFINED_BEHAVIOR"; 
        break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:         
        typeFmt = "PORTABILITY"; 
        break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:         
        typeFmt = "PERFORMANCE"; 
        break;
    case GL_DEBUG_TYPE_OTHER_ARB:               
        typeFmt = "OTHER"; 
        break;
    }

#ifdef _WIN32
    OutputDebugString("Type: ");
    OutputDebugString(typeFmt);
    OutputDebugString("\n");
#endif
    cout << "Type: " << typeFmt << endl;

    const char *severityFmt = "UNDEFINED";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        severityFmt = "HIGH";   
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        severityFmt = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
        severityFmt = "LOW"; 
        break;
    }

#ifdef _WIN32
    OutputDebugString("Severity: ");
    OutputDebugString(severityFmt);
    OutputDebugString("\n");

    OutputDebugString("Id: ");
    OutputDebugString(to_string(id).c_str());
    OutputDebugString("\n");

    OutputDebugString("Message: ");
    OutputDebugString(message);
    OutputDebugString("\n");
#endif
    cout << "Severity: " << severityFmt << endl;
    cout << "Id:" << id << endl;
    cout << "Message:" << message << endl;
}

void gameDebugCallbackAMD(GLuint id, GLenum category, GLenum severity,
                          GLsizei length, const GLchar *message, 
                          GLvoid* userParam)
{
#ifdef _WIN32
    OutputDebugString("AMD DEBUG CALLBACK:\n");
#endif
    cout << "AMD DEBUG CALLBACK:\n" << endl;

    const char *categoryFmt = "UNDEFINED";
    switch(category)
    {
    case GL_DEBUG_CATEGORY_API_ERROR_AMD:               
        categoryFmt = "API"; 
        break;
    case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD: 
        categoryFmt = "WINDOW SYSTEM"; 
        break;
    case GL_DEBUG_CATEGORY_DEPRECATION_AMD:  
        categoryFmt = "DEPRECATION"; 
        break;
    case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:         
        categoryFmt = "UNDEFINED BEHAVIOR"; 
        break;
    case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:         
        categoryFmt = "PERFORMANCE"; 
        break;
    case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:               
        categoryFmt = "SHADER COMPILER"; 
        break;
    case GL_DEBUG_CATEGORY_APPLICATION_AMD:               
        categoryFmt = "APPLICATION"; 
        break;
    case GL_DEBUG_CATEGORY_OTHER_AMD:
        categoryFmt = "OTHER"; 
        break;
    }

#ifdef _WIN32
    OutputDebugString("Category: ");
    OutputDebugString(categoryFmt);
    OutputDebugString("\n");
#endif
    cout << "Category: " << categoryFmt << endl;
 
    const char *severityFmt = "UNDEFINED";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_AMD:
        severityFmt = "HIGH";   
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_AMD:
        severityFmt = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW_AMD:
        severityFmt = "LOW"; 
        break;
    }

#ifdef _WIN32
    OutputDebugString("Severity: ");
    OutputDebugString(severityFmt);
    OutputDebugString("\n");

    OutputDebugString("Id: ");
    OutputDebugString(to_string(id).c_str());
    OutputDebugString("\n");

    OutputDebugString("Message: ");
    OutputDebugString(message);
    OutputDebugString("\n");
#endif
    cout << "Severity: " << severityFmt << endl;
    cout << "Id:" << id << endl;
    cout << "Message:" << message << endl;
}

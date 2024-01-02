#include "Tools.h"
#include <XorStr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_additional.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/android_native_app_glue.h"
#include "imgui/fonts/GoogleSans.h"

#include <unordered_set>
#include <GLES/gl.h>
#include <dlfcn.h>

#include "xhook/xhook.h"
#include "UserLogin.hpp"
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...
uintptr_t nikhil,g_LocalPlayer = 0, g_LocalController = 0, g_LocalWeapon = 0;

uintptr_t UE4;
uintptr_t anogs;
uintptr_t anort;
uintptr_t hdmpve;

int screenWidth = -1, glWidth, screenHeight = -1, glHeight;
float density = -1;
bool initImGui = false;
bool getapi = true;/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...
#define ASTExtraBaseCharacter_WeaponManagerComponent  0x21b8
#define ASTExtraShootWeapon_ShootWeaponEntityComp 0xfe8
#define UWeaponManagerComponent_CurrentWeaponReplicated  0x4f0
#define ASTExtraShootWeapon_ShootWeaponEffectComp 0xff0

#define __bgmi

#if defined(__bgmi)
    #if defined(__LP64__) // Battlegrounds Mobile India
        #define GNativeAndroidApp_Offset 0xbe30cb8
    #else
        #define GNativeAndroidApp_Offset 0x86cf0f8
    #endif
#else
    #if defined(__LP64__) // PUBGM (GL/KR/TW/VN)
        #define GNativeAndroidApp_Offset 0xC05A0B8
    #else
        #define GNativeAndroidApp_Offset 0x896f0f8
    #endif
#endif
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

const char *newPlayerMaskShader =
        "#version 310 es\n"
        "\n"
        "\n"
        "#ifdef GL_EXT_gpu_shader5\n"
        "#extension GL_EXT_gpu_shader5 : enable\n"
        "\n"
        "#endif\n"
        "\n"
        "#ifdef GL_EXT_texture_buffer\n"
        "#extension GL_EXT_texture_buffer : enable\n"
        "\n"
        "#endif\n"
        "\n"
        "#ifdef GL_EXT_texture_cube_map_array\n"
        "#extension GL_EXT_texture_cube_map_array : enable\n"
        "\n"
        "#endif\n"
        "\n"
        "#ifdef GL_EXT_shader_io_blocks\n"
        "#extension GL_EXT_shader_io_blocks : enable\n"
        "\n"
        "#endif\n"
        "#define INTERFACE_LOCATION(Pos) layout(location=Pos) \n"
        "#define INTERFACE_BLOCK(Pos, Interp, Modifiers, Semantic, PreType, PostType) layout(location=Pos) Modifiers "
        "Semantic { PreType PostType; }\n"
        "#define HLSLCC_DX11ClipSpace 1 \n"
        "#define HLSLCC_DoZConvertion 1 \n"
        "\n"
        "// end extensions\n"
        "INTERFACE_BLOCK(0, smooth , in , TEXCOORD10, highp vec4, Data) in_TEXCOORD10;\n"
        "INTERFACE_BLOCK(1, smooth , in , TEXCOORD11, highp vec4, Data) in_TEXCOORD11;\n"
        "INTERFACE_BLOCK(3, smooth , in , TEXCOORD0, highp vec2, Data) in_TEXCOORD0;\n"
        "INTERFACE_BLOCK(5, smooth , in , TEXCOORD8, highp vec4, Data) in_TEXCOORD8;\n"
        "precision mediump float;\n"
        "precision mediump int;\n"
        "\n"
        "#ifndef DONTEMITSAMPLERDEFAULTPRECISION\n"
        "precision mediump sampler2D;\n"
        "precision mediump samplerCube;\n"
        "\n"
        "#endif\n"
        "\n"
        "#ifdef TEXCOORDPRECISIONWORKAROUND\n"
        "vec4 texture2DTexCoordPrecisionWorkaround(sampler2D p, vec2 tcoord)\n"
        "{\n"
        "  return texture2D(p, tcoord);\n"
        "}\n"
        "#define texture2D texture2DTexCoordPrecisionWorkaround\n"
        "#endif\n"
        "\n"
        "void compiler_internal_AdjustInputSemantic(inout vec4 TempVariable)\n"
        "{\n"
        "#if HLSLCC_DX11ClipSpace\n"
        "  TempVariable.y = -TempVariable.y;\n"
        "#if HLSLCC_DoZConvertion\n"
        "  TempVariable.z = ( TempVariable.z + TempVariable.w ) / 2.0;\n"
        "#endif\n"
        "#endif\n"
        "}\n"
        "\n"
        "void compiler_internal_AdjustOutputSemantic(inout vec4 Src)\n"
        "{\n"
        "#if HLSLCC_DX11ClipSpace\n"
        "  Src.y = -Src.y;\n"
        "#if HLSLCC_DoZConvertion\n"
        "  Src.z = ( 2.0 * Src.z ) - Src.w;\n"
        "#endif\n"
        "#endif\n"
        "}\n"
        "\n"
        "bool compiler_internal_AdjustIsFrontFacing(bool isFrontFacing)\n"
        "{\n"
        "#if HLSLCC_DX11ClipSpace\n"
        "  return !isFrontFacing;\n"
        "#else\n"
        "  return isFrontFacing;\n"
        "#endif\n"
        "}\n"
        "uniform vec4 pc0_m[2];\n"
        "uniform highp vec4 pc0_h[7];\n"
        "uniform vec4 pc1_m[5];\n"
        "uniform vec4 pc2_m[10];\n"
        "uniform vec4 pu_m[7];\n"
        "uniform highp vec4 pu_h[2];\n"
        "uniform highp samplerCube ps2;\n"
        "uniform highp sampler2D ps1;\n"
        "uniform highp sampler2D ps0;\n"
        "INTERFACE_BLOCK(4, smooth , in , TEXCOORD7, vec4, Data) in_TEXCOORD7;\n"
        "INTERFACE_LOCATION(0) out vec4 out_Target0;\n"
        "void main()\n"
        "{\n"
        "  out_Target0 = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

std::unordered_set<GLuint> playerPrograms;
GLuint playerVertexShader = 0, playerMaskShader = 0;

std::mutex playerProgramsMutex;
std::mutex playerShaderMutex;
bool small = false;
struct sConfig_t {
	bool Bypass = true;
	bool ENABLE1 = false;
    struct schams {
        bool wallhack;
        bool enabled;
        bool rainbow;
        bool colorfull;
        bool glow;
        bool wireframe;
    };
    schams chams;
    struct sColors {
        float *color;
    };
    sColors colors{0};
};
sConfig_t config_t{0};
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

#define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

struct sRegion {
    uintptr_t start, end;
};

std::vector<sRegion> trapRegions;

void DrawText(std::string text) {
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
    ImGui::Text(text.c_str());
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (glWidth <= 0 || glHeight <= 0)
        return orig_eglSwapBuffers(dpy, surface);
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

    if (!g_App)
        return orig_eglSwapBuffers(dpy, surface);

    screenWidth = ANativeWindow_getWidth(g_App->window);
    screenHeight = ANativeWindow_getHeight(g_App->window);
    density = AConfiguration_getDensity(g_App->config);

    if (!initImGui) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiStyle *style = &ImGui::GetStyle();
        
		style->WindowPadding = ImVec2(10,9);
            style->FramePadding = ImVec2(3,3);
			style->FrameBorderSize = 0.5f;
			style->TabRounding = 4.0f;
			style->GrabRounding = 5.0f;
		
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

    ImVec4 *colors = style->Colors;
    colors[ImGuiCol_Text]                  = ImColor(255, 255, 255, 255);
        colors[ImGuiCol_WindowBg]              = ImColor(255, 0, 0, 110);
        colors[ImGuiCol_Border]                = ImVec4(0,0,0,255);
        colors[ImGuiCol_FrameBg]               = ImColor(39, 172, 57, 110);
        colors[ImGuiCol_FrameBgActive]         = ImColor(87, 143, 255, 15);
        colors[ImGuiCol_FrameBgHovered]        = ImColor(255, 0, 0, 110);
        colors[ImGuiCol_TitleBg]               = ImColor(39, 172, 57, 110);
        colors[ImGuiCol_TitleBgActive]         = ImColor(39, 172, 57, 110);
        colors[ImGuiCol_TitleBgCollapsed]      = ImColor(39, 172, 57, 110);
        colors[ImGuiCol_CheckMark]             = ImColor(255, 255, 255, 255);
        colors[ImGuiCol_Separator]             = ImColor(0.70f, 0.70f, 0.70f, 0.62f);
        colors[ImGuiCol_SeparatorActive]       = ImColor(0.30f, 0.30f, 0.30f, 0.84f);
        colors[ImGuiCol_SeparatorHovered]      = ImColor(255, 0, 0, 110);
		colors[ImGuiCol_MenuBarBg]             = ImVec4(1.0f, 1.0f, 1.0f, 0.80f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
        colors[ImGuiCol_Button]                = ImColor(255, 0, 0, 110);
        colors[ImGuiCol_ButtonActive]          = ImColor(255, 0, 0, 110);
        colors[ImGuiCol_ButtonHovered]         = ImColor(255, 0, 0, 110);
        colors[ImGuiCol_HeaderHovered]         = ImColor(46, 46, 46, 255);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
     
	  style->ScaleAllSizes(std::max(2.5f, density / 400.0f));
        style->ScrollbarSize /= 1;
        /////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");

       //  ImGuiIO &io = ImGui::GetIO();

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
        io.MouseDrawCursor = true;

        ImFontConfig cfg;
        cfg.SizePixels = ((float) density / 24.0f);
        io.Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, 24.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

        config_t.colors.color = CREATE_COLOR(1.0f, 0.0f, 0.0f, 1.0f);

        initImGui = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
           ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
           ImGui::SetNextWindowSize(ImVec2((float) glWidth * 0.28f, (float) glHeight * 0.55f), ImGuiCond_Once);

        if (ImGui::Begin(_enc("CAPTAINSRC CONFIG MOD 2.9.0"), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
		static bool isLogin = false;
		if (!isLogin) {
			static std::string err;
			ImGui::Text("Please Login! (Copy Key to Clipboard)");
			ImGui::PushItemWidth(-1);
			static char s[64];
			ImGui::InputText("##key", s, sizeof s);
			// auto paste
			auto key = getClipboardText();
			strncpy(s, key.c_str(), sizeof s);
			// auto login
			err = Login(s);
			if (err == "OK") {
				isLogin = bValid && g_Auth == g_Token;
			}
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(-1);
			if (ImGui::Button(" Paste Key  ", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
				auto key = getClipboardText();
				strncpy(s, key.c_str(), sizeof s);
			}
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(-1);
			if (ImGui::Button("Login", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
				err = Login(s);
				if (err == "OK") {
					isLogin = bValid && g_Auth == g_Token;
				}
			}
			ImGui::PopItemWidth();
			if (!err.empty() && err != "OK") {
				ImGui::Text("Error: %s", err.c_str());
			}
			ImGui::PopItemWidth();
        } else {
            if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {
                if (ImGui::BeginTabItem("Chams")) {
                    ImGui::Spacing();
                    ImGui::Text("Chams Menu:");

                    if (ImGui::BeginTable("split", 2));
                    {
                        ImGui::TableNextColumn(); ImGui::Checkbox("WallHack", &config_t.chams.wallhack);
                        ImGui::TableNextColumn(); ImGui::Checkbox("GreenBody", &config_t.chams.enabled);
						ImGui::TableNextColumn(); ImGui::Checkbox("No Recoil", &config_t.ENABLE1);
                        ImGui::TableNextColumn(); ImGui::Checkbox("Small Cross", &small);
                        /*ImGui::TableNextColumn(); ImGui::Checkbox("Rainbow", &config_t.chams.rainbow);
                        ImGui::TableNextColumn(); ImGui::Checkbox("ColorFull", &config_t.chams.colorfull);
                        ImGui::TableNextColumn(); ImGui::Checkbox("Glow", &config_t.chams.glow);
                        ImGui::TableNextColumn(); ImGui::Checkbox("Wireframe", &config_t.chams.wireframe);*/
                        ImGui::EndTable();
                    }

                    ImGui::Text("Chams Color:");
                    ImGui::ColorEdit3("##Chams", config_t.colors.color);
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Info")) {
                    ImGui::Separator();
                    DrawText(_enc("Published On: @VipBadalOfficial"));
					DrawText(_enc("Channel On: @CAPTAINSRC"));
                    ImGui::Separator();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
}

int32_t (*orig_onInputEvent)(struct android_app *app, AInputEvent *inputEvent);
int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent) {
    if (initImGui) {
        ImGui_ImplAndroid_HandleInputEvent(inputEvent, {(float) screenWidth / (float) glWidth, (float) screenHeight / (float) glHeight});
    }
    return orig_onInputEvent(app, inputEvent);
}

void (*oglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void _glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
    if (config_t.chams.wallhack) {
        GLuint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *) &currentProgram);
        {
            std::lock_guard<std::mutex> lock(playerProgramsMutex);
            if (playerPrograms.find(currentProgram) == playerPrograms.end()) {
                return oglDrawElements(mode, count, type, indices);
            }
        }
        GLfloat depthRange[2];
        glGetFloatv(GL_DEPTH_RANGE, depthRange);
        GLboolean colorWriteMask[4];
        glGetBooleanv(GL_COLOR_WRITEMASK, colorWriteMask);
        GLboolean blend;
        glGetBooleanv(GL_BLEND, &blend);
        if (!blend) {
            glEnable(GL_BLEND);
        }
        glDepthRangef(1.f, 0.0f);
        glColorMask(1, 1, 1, 1);
        if (config_t.chams.enabled) {
            GLint blendSrc, blendDst;
            glGetIntegerv(GL_BLEND_SRC, &blendSrc);
            glGetIntegerv(GL_BLEND_DST, &blendDst);
            GLfloat blendColor[4];
            glGetFloatv(GL_BLEND_COLOR, blendColor);
            glBlendFunc(GL_CONSTANT_COLOR, GL_ZERO);
            if (config_t.chams.rainbow) {
                static float cnt = 0.0f;

                float r = cos(cnt) * .5f + .5f;
                float g = cos(cnt - 2.f * 3.14 / 3.f) * .5f + .5f;
                float b = cos(cnt - 4.f * 3.14 / 3.f) * .5f + .5f;

                if (cnt >= FLT_MAX) {
                    cnt = 0.0f;
                } else {
                    cnt += 0.01f;
                }
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

                glBlendColor(r, g, b, 1.0f);
            } else {
                glBlendColor(config_t.colors.color[1], config_t.colors.color[0], config_t.colors.color[1], 1.0f);
            }
            oglDrawElements(mode, count, type, indices);

            glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[3]);
            glBlendFunc(blendSrc, blendDst);
        } else {
            oglDrawElements(mode, count, type, indices);
        }
        if (config_t.chams.colorfull) {
            static float cnt = 0.0f;
            float r = cos(cnt) * .5f + .5f;
            float g = cos(cnt - 2.f * 3.14 / 3.f) * .5f + .5f;
            float b = cos(cnt - 4.f * 3.14 / 3.f) * .5f + .5f;
            if (cnt >= FLT_MAX) {
                    cnt = 0.0f;
            } else {
                    cnt += 0.01f;
            }
            glDepthRangef(1, 0);
            glEnable(GL_BLEND);
            glColorMask(1, 1, 1, 1);
            glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
            glBlendColor(0, 0, 0, 1);
            oglDrawElements(GL_TRIANGLES, count, type, indices);
            glDepthRangef(0.5, 1);
            glBlendColor(r, g, b, 1.0f);
            oglDrawElements(GL_LINES, count, type, indices);
        } else {
            glBlendColor(config_t.colors.color[1], config_t.colors.color[0], config_t.colors.color[1], 1.0f);
        }
        if (config_t.chams.glow) {
            glEnable(GL_BLEND);
            glColorMask(1, 1, 1, 1);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
            glLineWidth(1);
            glDepthRangef(0.5, 1);
            oglDrawElements(GL_LINES, count, type, indices);
            glBlendColor(1, 1, 1, 1);
            glDepthRangef(1, 0.5);
            oglDrawElements(GL_TRIANGLES, count, type, indices);
        } else {
            glBlendColor(config_t.colors.color[1], config_t.colors.color[0], config_t.colors.color[1], 1.0f);
        }
        if (config_t.chams.wireframe) {
            glDepthRangef(1, 0);
            glLineWidth(1);
            glEnable(GL_BLEND);
            glColorMask(1, 1, 1, 1);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
            glBlendColor(0, 0, 0, 1);
            oglDrawElements(GL_TRIANGLES, count, type, indices);
            glDepthRangef(1, 0.5);
            glBlendColor(1, 2, 2, 1);
            oglDrawElements(GL_LINES, count, type, indices);
        } else {
            glBlendColor(config_t.colors.color[1], config_t.colors.color[0], config_t.colors.color[1], 1.0f);
        }
        
        glDepthRangef(depthRange[0], depthRange[1]);
        glColorMask(colorWriteMask[0], colorWriteMask[1], colorWriteMask[2], colorWriteMask[3]);
        if (!blend) {
            glDisable(GL_BLEND);
        }
        return;
    }
    return oglDrawElements(mode, count, type, indices);
}

void (*oglShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
void _glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
    if (!string || !*string) {
        return oglShaderSource(shader, count, string, length);
    }
    if (!count) {
        return oglShaderSource(shader, count, string, length);
    }
// Battlegrounds Mobile India
     if (strstr(*string,"Master_Mask_Base/TGPUSkinVertexFactorytrue") || strstr(*string,"Master_Mask_Base_Player/TGPUSkinVertexFactorytrue") || strstr(*string,"Master_VH_IBL/TGPUSkinVertexFactorytrue") || strstr(*string,"Master_VH_Base/TGPUSkinVertexFactorytrue") || strstr(*string,"Master_Weapon_Player/TGPUSkinVertexFactorytrue")) {
// PUBG MOBILE
   // if (strstr(*string, "Master_Mask_Base/TGPUSkinVertexFactorytrue") || strstr(*string, "Master_Mask_Base/TGPUSkinVertexFactoryfalse")) {
        GLenum shaderType = 0;
        glGetShaderiv(shader, GL_SHADER_TYPE, reinterpret_cast<GLint *>(&shaderType));
        if (shaderType == GL_VERTEX_SHADER) {
            std::lock_guard<std::mutex> lock{playerShaderMutex};
            playerVertexShader = shader;
            *string = newPlayerMaskShader;
        }
    }
// Battlegrounds Mobile India
     if (strstr(*string,"Master_Mask_Base/TGPUSkinVertexFactoryfalse") || strstr(*string,"Master_Mask_Base_Player/TGPUSkinVertexFactoryfalse") || strstr(*string,"Master_VH_IBL/TGPUSkinVertexFactoryfalse") || strstr(*string, "Master_VH_Base/TGPUSkinVertexFactoryfalse") || strstr(*string,"Master_Weapon_Player/TGPUSkinVertexFactoryfalse")) {
// PUBG MOBILE
  //  if (strstr(*string, "Master_Mask_Base/TGPUSkinVertexFactorytrue") || strstr(*string, "Master_Mask_Base/TGPUSkinVertexFactoryfalse")) {
        GLenum shaderTypes = 0;
        glGetShaderiv(shader, GL_SHADER_TYPE, reinterpret_cast<GLint *>(&shaderTypes));
        if (shaderTypes == GL_FRAGMENT_SHADER) {
            std::lock_guard<std::mutex> lock{playerShaderMutex};
            playerMaskShader = shader;
            *string = newPlayerMaskShader;
        }
    }
    return oglShaderSource(shader, count, string, length);
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

void (*oglAttachShader)(GLuint program, GLuint shader);
void _glAttachShader(GLuint program, GLuint shader) {
    std::unique_lock<std::mutex> ulock{playerShaderMutex};
    if (shader == playerVertexShader || shader == playerMaskShader) {
        ulock.unlock();
        {
            std::lock_guard<std::mutex> lock{playerProgramsMutex};
            playerPrograms.insert(program);
        }
    }
    return oglAttachShader(program, shader);
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

template<typename T>
void MIRI_MEM(uintptr_t addr, T value) {
    Tools::PVM_WriteAddr((void *) addr, &value, sizeof(T));
}
#define SLEEP_TIME 1000LL / 60LL

[[noreturn]] void *maps_thread(void *) {
    while (true) {
        auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        std::vector<sRegion> tmp;
        char line[512];
        FILE *f = fopen("/proc/self/maps", "r");
        if (f) {
            while (fgets(line, sizeof line, f)) {
                uintptr_t start, end;
                char tmpProt[16];
                if (sscanf(line, "%" PRIXPTR "-%" PRIXPTR " %16s %*s %*s %*s %*s", &start, &end, tmpProt) > 0) {
                    if (tmpProt[0] != 'r') {
                        tmp.push_back({start, end});
                    }
                }
            }
            fclose(f);
        }
        trapRegions = tmp;
		
		if(small){
Tools::WriteAddr((void *) (UE4 + 0x5297258), (void *) "00 00 00 00", 4);
}
auto miri_local = g_LocalPlayer;
auto miri_control = g_LocalController;
if (miri_local != 0 && miri_control) {
                auto WeaponManagerComponent = * (uintptr_t *)(miri_local + ASTExtraBaseCharacter_WeaponManagerComponent);
                if (WeaponManagerComponent) {
                auto CurrentWeaponReplicated = * (uintptr_t *)(WeaponManagerComponent + UWeaponManagerComponent_CurrentWeaponReplicated);
                if (CurrentWeaponReplicated) {
                auto ShootWeaponEntityComp = * (uintptr_t *)(CurrentWeaponReplicated + ASTExtraShootWeapon_ShootWeaponEntityComp);
                auto ShootWeaponEffectComp = * (uintptr_t *)(CurrentWeaponReplicated + ASTExtraShootWeapon_ShootWeaponEffectComp);
                if (ShootWeaponEntityComp && ShootWeaponEffectComp) {  
       if (config_t.ENABLE1) {
		   MIRI_MEM<float>(ShootWeaponEffectComp + 0x258, 0);
           MIRI_MEM<float>(ShootWeaponEffectComp + 0x25c, 0);
           MIRI_MEM<float>(ShootWeaponEffectComp + 0x260, 0);
		   MIRI_MEM<float>(ShootWeaponEntityComp + 0xb40,0);
           MIRI_MEM<float>(ShootWeaponEntityComp + 0xb30, 0);
           MIRI_MEM<float>(ShootWeaponEntityComp + 0xb34, 0);
           MIRI_MEM<float>(ShootWeaponEntityComp + 0xb38, 0);
	    }
                     }
                 }
             }
         }
		

        auto td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - t1;
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(std::min(0LL, SLEEP_TIME - td), SLEEP_TIME)));
    }
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

void *main_thread(void *) {
     UE4 = Tools::GetBaseAddress("libUE4.so");
    while (!UE4) {
        UE4 = Tools::GetBaseAddress("libUE4.so");
        sleep(1);
	if (config_t.Bypass) {
//Yahan per Tumhara bypass lagao & use
    } 
	}

    while (!g_App) {
        g_App = *(android_app **) (UE4 + GNativeAndroidApp_Offset);
        sleep(1);
    }

    while (!g_App->onInputEvent)
        sleep(1);
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

    orig_onInputEvent = decltype(orig_onInputEvent)(g_App->onInputEvent);
    g_App->onInputEvent = onInputEvent;

    xhook_register(".*\\.so$", "eglSwapBuffers", (void*)_eglSwapBuffers, (void **)&orig_eglSwapBuffers);
    xhook_register(".*\\.so$", "glDrawElements", (void*)_glDrawElements, (void **)&oglDrawElements);
    xhook_register(".*\\.so$", "glShaderSource", (void*)_glShaderSource, (void **)&oglShaderSource);
    xhook_register(".*\\.so$", "glAttachShader", (void*)_glAttachShader, (void **)&oglAttachShader);
    xhook_refresh(0);

    pthread_t t;
    pthread_create(&t, 0, maps_thread, 0);
    return 0;
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

__attribute__((constructor)) void _init() {
    pthread_t t;
    pthread_create(&t, 0, main_thread, 0);
    system("su -c setenforce 0");
    system("su -lp 2000 -c cmd notification post -S bigtext -t Karnish_WALLHACK Tag WELLCOME_USER &> /dev/null");
}

extern "C" {
JNIEXPORT void JNICALL
Java_androidx_multidex_MultDex_getapi(JNIEnv *, jobject, jint code,
jboolean jboolean1) {
switch ((int)code) {      

case 267:
getapi = jboolean1;
}
}
}
/////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC.../////////////// CAPTAINSRC \\\\\\\\\ SRC...

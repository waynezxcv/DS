cmake_minimum_required(VERSION 3.6)
project(DispatchSocketDemo_xcodeproj)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        project.xcworkspace/xcuserdata/liuwei.xcuserdatad/UserInterfaceState.xcuserstate
        project.xcworkspace/xcuserdata/wayne.xcuserdatad/UserInterfaceState.xcuserstate
        project.xcworkspace/contents.xcworkspacedata
        xcuserdata/liuwei.xcuserdatad/xcdebugger/Breakpoints_v2.xcbkptlist
        xcuserdata/liuwei.xcuserdatad/xcschemes/DispatchSocketDemo.xcscheme
        xcuserdata/liuwei.xcuserdatad/xcschemes/xcschememanagement.plist
        xcuserdata/wayne.xcuserdatad/xcdebugger/Breakpoints_v2.xcbkptlist
        xcuserdata/wayne.xcuserdatad/xcschemes/DispatchSocketDemo.xcscheme
        xcuserdata/wayne.xcuserdatad/xcschemes/xcschememanagement.plist
        project.pbxproj)

add_executable(DispatchSocketDemo_xcodeproj ${SOURCE_FILES})
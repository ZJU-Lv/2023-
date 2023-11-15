# 2023三维CAD建模大程作业

Github项目网址：https://github.com/ZJU-Lv/2023CAD-Final-Project

## 环境配置

项目已配置好相对路径，理论上用vs（我的版本是2019）打开sln文件后可以直接编译运行。

若不能成功运行，请按以下方法配置项目包含路径：

1、Platform选择Active(Win32)。

2、VC++ Directories下Include Directories添加下述目录。

```
$(SolutionDir)Dependencies\GLEW\include
$(SolutionDir)Dependencies\GLFW\include
$(SolutionDir)Dependencies\GLUT
```

3、Linker->General->Additional Library Directories添加下述目录。

```
$(SolutionDir)Dependencies\GLEW\lib\Release\Win32
$(SolutionDir)Dependencies\GLFW\lib-vc2019
$(SolutionDir)Dependencies\GLUT
```

4、Linker->Input->Additional Dependencies添加下述内容。

```
glfw3.lib
glew32.lib
glew32s.lib
glut.lib
glut32.lib
```

之后直接运行项目即可。

## 操作方式

使用键盘wsad四个按键可以控制几何体的旋转。

## 效果展示

初始效果![image-20231114201157128](C:\Users\12944\AppData\Roaming\Typora\typora-user-images\image-20231114201157128.png)

旋转效果
![image-20231114201224945](C:\Users\12944\AppData\Roaming\Typora\typora-user-images\image-20231114201224945.png)![image-20231114201302188](C:\Users\12944\AppData\Roaming\Typora\typora-user-images\image-20231114201302188.png)
![image-20231114201418569](C:\Users\12944\AppData\Roaming\Typora\typora-user-images\image-20231114201418569.png)
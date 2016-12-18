xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 185;
 0.18799;2.92056;7.13641;,
 0.18799;2.70020;7.13641;,
 0.57449;2.70020;7.13641;,
 0.55231;2.84945;7.13641;,
 -0.19293;2.92056;7.13641;,
 -0.55330;2.84945;7.13641;,
 -0.57549;2.70020;7.13641;,
 -0.19293;2.70020;7.13641;,
 -0.36657;2.81086;7.13641;,
 -0.48027;2.81086;7.13641;,
 -0.48027;2.81086;7.21984;,
 -0.36657;2.81086;7.21984;,
 -0.36657;2.81086;7.21984;,
 -0.48027;2.81086;7.21984;,
 -0.48027;2.74251;7.21984;,
 -0.36657;2.74251;7.21984;,
 -0.36657;2.74251;7.21984;,
 -0.48027;2.74251;7.21984;,
 -0.48027;2.74251;7.13641;,
 -0.36657;2.74251;7.13641;,
 -0.48027;2.81086;7.13641;,
 -0.48027;2.74251;7.13641;,
 -0.48027;2.74251;7.21984;,
 -0.48027;2.81086;7.21984;,
 -0.36657;2.74251;7.13641;,
 -0.36657;2.81086;7.13641;,
 -0.36657;2.81086;7.21984;,
 -0.36657;2.74251;7.21984;,
 0.36657;2.81086;7.13641;,
 0.36657;2.81086;7.21984;,
 0.48027;2.81086;7.21984;,
 0.48027;2.81086;7.13641;,
 0.36657;2.81086;7.21984;,
 0.36657;2.74251;7.21984;,
 0.48027;2.74251;7.21984;,
 0.48027;2.81086;7.21984;,
 0.36657;2.74251;7.21984;,
 0.36657;2.74251;7.13641;,
 0.48027;2.74251;7.13641;,
 0.48027;2.74251;7.21984;,
 0.48027;2.81086;7.13641;,
 0.48027;2.81086;7.21984;,
 0.48027;2.74251;7.21984;,
 0.48027;2.74251;7.13641;,
 0.36657;2.74251;7.13641;,
 0.36657;2.74251;7.21984;,
 0.36657;2.81086;7.21984;,
 0.36657;2.81086;7.13641;,
 -0.08906;2.85679;7.25687;,
 -0.04279;2.83762;7.25687;,
 -0.04279;2.83762;7.13641;,
 -0.08906;2.85679;7.13641;,
 -0.02362;2.79135;7.25687;,
 -0.02362;2.79135;7.13641;,
 -0.04279;2.74508;7.25687;,
 -0.04279;2.74508;7.13641;,
 -0.08906;2.72591;7.25687;,
 -0.08906;2.72591;7.13641;,
 -0.13533;2.74508;7.25687;,
 -0.13533;2.74508;7.13641;,
 -0.15450;2.79135;7.25687;,
 -0.15450;2.79135;7.13641;,
 -0.13533;2.83762;7.25687;,
 -0.13533;2.83762;7.13641;,
 -0.02133;2.86011;7.25695;,
 -0.15867;2.86011;7.25695;,
 -0.15867;2.72277;7.25695;,
 -0.02133;2.72277;7.25695;,
 -0.26906;2.85679;7.25687;,
 -0.22279;2.83762;7.25687;,
 -0.22279;2.83762;7.13641;,
 -0.26906;2.85679;7.13641;,
 -0.20362;2.79135;7.25687;,
 -0.20362;2.79135;7.13641;,
 -0.22279;2.74508;7.25687;,
 -0.22279;2.74508;7.13641;,
 -0.26906;2.72591;7.25687;,
 -0.26906;2.72591;7.13641;,
 -0.31533;2.74508;7.25687;,
 -0.31533;2.74508;7.13641;,
 -0.33450;2.79135;7.25687;,
 -0.33450;2.79135;7.13641;,
 -0.31533;2.83762;7.25687;,
 -0.31533;2.83762;7.13641;,
 -0.20133;2.86011;7.25695;,
 -0.33867;2.86011;7.25695;,
 -0.33867;2.72277;7.25695;,
 -0.20133;2.72277;7.25695;,
 0.09094;2.85679;7.25687;,
 0.13721;2.83762;7.25687;,
 0.13721;2.83762;7.13641;,
 0.09094;2.85679;7.13641;,
 0.15638;2.79135;7.25687;,
 0.15638;2.79135;7.13641;,
 0.13721;2.74508;7.25687;,
 0.13721;2.74508;7.13641;,
 0.09094;2.72591;7.25687;,
 0.09094;2.72591;7.13641;,
 0.04467;2.74508;7.25687;,
 0.04467;2.74508;7.13641;,
 0.02550;2.79135;7.25687;,
 0.02550;2.79135;7.13641;,
 0.04467;2.83762;7.25687;,
 0.04467;2.83762;7.13641;,
 0.15867;2.86011;7.25695;,
 0.02133;2.86011;7.25695;,
 0.02133;2.72277;7.25695;,
 0.15867;2.72277;7.25695;,
 0.27094;2.85679;7.25687;,
 0.31721;2.83762;7.25687;,
 0.31721;2.83762;7.13641;,
 0.27094;2.85679;7.13641;,
 0.33638;2.79135;7.25687;,
 0.33638;2.79135;7.13641;,
 0.31721;2.74508;7.25687;,
 0.31721;2.74508;7.13641;,
 0.27094;2.72591;7.25687;,
 0.27094;2.72591;7.13641;,
 0.22467;2.74508;7.25687;,
 0.22467;2.74508;7.13641;,
 0.20550;2.79135;7.25687;,
 0.20550;2.79135;7.13641;,
 0.22467;2.83762;7.25687;,
 0.22467;2.83762;7.13641;,
 0.33867;2.86011;7.25695;,
 0.20133;2.86011;7.25695;,
 0.20133;2.72277;7.25695;,
 0.33867;2.72277;7.25695;,
 0.53742;2.88218;7.08560;,
 0.35027;2.92979;7.08560;,
 -0.35027;2.92979;7.08560;,
 -0.53742;2.88218;7.08560;,
 0.17794;2.94337;7.08560;,
 -0.17794;2.94337;7.08560;,
 0.00000;2.95284;7.08560;,
 0.57454;2.70020;7.08560;,
 0.38579;2.68686;7.75755;,
 0.57975;2.68838;7.66400;,
 0.56654;2.79674;7.38220;,
 -0.57454;2.70020;7.08560;,
 -0.57975;2.68838;7.66400;,
 -0.38579;2.68686;7.75755;,
 -0.56654;2.79674;7.38220;,
 0.00000;2.68686;7.81222;,
 0.19505;2.68686;7.78909;,
 -0.19505;2.68686;7.78909;,
 -0.56654;2.79674;7.38220;,
 -0.53742;2.88218;7.08560;,
 -0.35027;2.92979;7.08560;,
 -0.37536;2.85140;7.35346;,
 -0.17794;2.94337;7.08560;,
 -0.18807;2.87052;7.34196;,
 0.00000;2.95284;7.08560;,
 0.00000;2.87746;7.34196;,
 0.56654;2.79674;7.38220;,
 0.37536;2.85140;7.35346;,
 0.35027;2.92979;7.08560;,
 0.53742;2.88218;7.08560;,
 0.18807;2.87052;7.34196;,
 0.17794;2.94337;7.08560;,
 -0.36657;2.81086;7.21984;,
 -0.48027;2.81086;7.21984;,
 -0.48027;2.74251;7.21984;,
 -0.36657;2.74251;7.21984;,
 0.36657;2.81086;7.21984;,
 0.36657;2.74251;7.21984;,
 0.48027;2.74251;7.21984;,
 0.48027;2.81086;7.21984;,
 0.57454;2.70020;7.08560;,
 0.38579;2.68686;7.75755;,
 0.57975;2.68838;7.66400;,
 -0.57454;2.70020;7.08560;,
 -0.57975;2.68838;7.66400;,
 -0.38579;2.68686;7.75755;,
 0.00000;2.68686;7.81222;,
 0.19505;2.68686;7.78909;,
 -0.19505;2.68686;7.78909;,
 0.57975;2.68838;7.66400;,
 0.56654;2.79674;7.38220;,
 0.53742;2.88218;7.08560;,
 0.57454;2.70020;7.08560;,
 -0.57975;2.68838;7.66400;,
 -0.57454;2.70020;7.08560;,
 -0.53742;2.88218;7.08560;,
 -0.56654;2.79674;7.38220;;
 
 75;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;4,7,1,0;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;49,52,53,50;,
 4;52,54,55,53;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 4;58,60,61,59;,
 4;60,62,63,61;,
 4;62,48,51,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;69,72,73,70;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,68,71,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;89,92,93,90;,
 4;92,94,95,93;,
 4;94,96,97,95;,
 4;96,98,99,97;,
 4;98,100,101,99;,
 4;100,102,103,101;,
 4;102,88,91,103;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;109,112,113,110;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 4;116,118,119,117;,
 4;118,120,121,119;,
 4;120,122,123,121;,
 4;122,108,111,123;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;129,132,133,130;,
 3;132,134,133;,
 3;135,136,137;,
 4;137,138,128,135;,
 3;139,140,141;,
 4;140,139,131,142;,
 3;139,143,135;,
 4;143,144,136,135;,
 4;143,139,141,145;,
 4;131,139,135,128;,
 4;146,147,148,149;,
 4;149,148,150,151;,
 4;151,150,152,153;,
 4;154,155,156,157;,
 4;155,158,159,156;,
 4;158,153,152,159;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 3;168,169,170;,
 3;171,172,173;,
 3;171,174,168;,
 4;174,175,169,168;,
 4;174,171,173,176;,
 4;177,178,179,180;,
 4;181,182,183,184;;
 
 MeshMaterialList {
  5;
  75;
  4,
  4,
  4,
  4,
  0,
  4,
  4,
  4,
  4,
  0,
  4,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "../mo_e5_1_u/light.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "../mo_e5_1_u/_light.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;0.990000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "../mo_e5_1_u/light21.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;0.990000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "../mo_e5_1_u/light22.png";
   }
  }
  Material {
   0.007000;0.007000;0.007000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  60;
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.707106;0.707107;0.000000;,
  1.000000;-0.000004;-0.000002;,
  0.707111;-0.707103;-0.000006;,
  -0.000000;-1.000000;-0.000009;,
  -0.707124;-0.707090;-0.000006;,
  -1.000000;-0.000004;-0.000002;,
  -0.707119;0.707094;0.000000;,
  0.000008;1.000000;0.000000;,
  0.707099;0.707115;0.000000;,
  1.000000;-0.000004;-0.000002;,
  0.707103;-0.707111;-0.000006;,
  0.000008;-1.000000;-0.000009;,
  -0.707124;-0.707090;-0.000006;,
  -1.000000;-0.000004;-0.000002;,
  -0.707119;0.707094;0.000000;,
  0.000008;1.000000;0.000000;,
  0.707099;0.707115;0.000000;,
  1.000000;-0.000004;-0.000002;,
  0.707103;-0.707111;-0.000006;,
  0.000008;-1.000000;-0.000009;,
  -0.707124;-0.707090;-0.000006;,
  -1.000000;-0.000004;-0.000002;,
  0.707099;0.707115;0.000000;,
  1.000000;-0.000004;-0.000002;,
  0.707103;-0.707111;-0.000006;,
  0.001660;0.999803;0.019766;,
  0.000000;0.999804;0.019801;,
  0.002920;0.999785;0.020527;,
  0.002490;0.999787;0.020473;,
  0.002060;0.999789;0.020420;,
  -0.988150;-0.151799;0.022738;,
  -0.001660;0.999803;0.019766;,
  -0.002490;0.999787;0.020473;,
  -0.002060;0.999789;0.020420;,
  0.988150;-0.151799;0.022738;,
  -0.002920;0.999785;0.020527;,
  0.060808;-0.959763;-0.274148;,
  -0.000000;-0.960848;-0.277077;,
  0.235100;-0.939219;-0.250193;,
  0.157228;-0.952277;-0.261624;,
  -0.060808;-0.959763;-0.274148;,
  -0.235100;-0.939219;-0.250193;,
  -0.157228;-0.952277;-0.261624;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.001660;0.999803;0.019766;,
  0.000000;0.999804;0.019801;,
  0.002920;0.999785;0.020527;,
  0.002490;0.999787;0.020473;,
  0.002060;0.999789;0.020420;,
  -0.001660;0.999803;0.019766;,
  -0.002490;0.999787;0.020473;,
  -0.002060;0.999789;0.020420;,
  -0.002920;0.999785;0.020527;,
  -0.988150;-0.151799;0.022738;,
  0.988150;-0.151799;0.022738;;
  75;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;47,47,47,47;,
  4;46,46,46,46;,
  4;1,3,3,1;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,1,1,9;,
  4;0,0,0,0;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,10,10,17;,
  4;0,0,0,0;,
  4;18,19,19,18;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,17,17,24;,
  4;17,18,18,17;,
  4;0,0,0,0;,
  4;10,25,25,10;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,14,14,27;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,10,10,17;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;28,31,32;,
  4;33,33,33,33;,
  3;34,36,35;,
  4;37,37,37,37;,
  3;34,29,28;,
  4;29,30,31,28;,
  4;29,34,35,38;,
  4;0,0,0,0;,
  4;41,41,42,42;,
  4;42,42,39,39;,
  4;39,39,40,40;,
  4;44,45,45,44;,
  4;45,43,43,45;,
  4;43,40,40,43;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  3;49,52,53;,
  3;54,56,55;,
  3;54,50,49;,
  4;50,51,52,49;,
  4;50,54,55,57;,
  4;58,58,58,58;,
  4;59,59,59,59;;
 }
 MeshTextureCoords {
  185;
  0.316460;1.000000;,
  0.325390;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.687380;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.678270;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.518110;0.196370;,
  0.984330;0.196370;,
  0.984330;0.791640;,
  0.518110;0.791640;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.518110;0.196370;,
  0.518110;0.791640;,
  0.984330;0.791640;,
  0.984330;0.196370;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.250490;0.040870;,
  0.088640;0.175630;,
  0.088640;0.175630;,
  0.250490;0.040870;,
  0.021600;0.500980;,
  0.021600;0.500980;,
  0.088640;0.826340;,
  0.088640;0.826320;,
  0.250490;0.961100;,
  0.250490;0.961100;,
  0.412330;0.826340;,
  0.412330;0.826320;,
  0.479360;0.500980;,
  0.479360;0.500980;,
  0.412330;0.175630;,
  0.412330;0.175630;,
  0.013580;0.017480;,
  0.493960;0.017480;,
  0.493960;0.983170;,
  0.013580;0.983170;,
  0.250490;0.040870;,
  0.088640;0.175630;,
  0.088640;0.175630;,
  0.250490;0.040870;,
  0.021600;0.500980;,
  0.021600;0.500980;,
  0.088640;0.826340;,
  0.088640;0.826320;,
  0.250490;0.961100;,
  0.250490;0.961100;,
  0.412330;0.826340;,
  0.412330;0.826320;,
  0.479360;0.500980;,
  0.479360;0.500980;,
  0.412330;0.175630;,
  0.412330;0.175630;,
  0.013580;0.017480;,
  0.493960;0.017480;,
  0.493960;0.983170;,
  0.013580;0.983170;,
  0.250490;0.040870;,
  0.088640;0.175630;,
  0.088640;0.175630;,
  0.250490;0.040870;,
  0.021600;0.500980;,
  0.021600;0.500980;,
  0.088640;0.826340;,
  0.088640;0.826320;,
  0.250490;0.961100;,
  0.250490;0.961100;,
  0.412330;0.826340;,
  0.412330;0.826320;,
  0.479360;0.500980;,
  0.479360;0.500980;,
  0.412330;0.175630;,
  0.412330;0.175630;,
  0.013580;0.017480;,
  0.493960;0.017480;,
  0.493960;0.983170;,
  0.013580;0.983170;,
  0.250490;0.040870;,
  0.088640;0.175630;,
  0.088640;0.175630;,
  0.250490;0.040870;,
  0.021600;0.500980;,
  0.021600;0.500980;,
  0.088640;0.826340;,
  0.088640;0.826320;,
  0.250490;0.961100;,
  0.250490;0.961100;,
  0.412330;0.826340;,
  0.412330;0.826320;,
  0.479360;0.500980;,
  0.479360;0.500980;,
  0.412330;0.175630;,
  0.412330;0.175630;,
  0.013580;0.017480;,
  0.493960;0.017480;,
  0.493960;0.983170;,
  0.013580;0.983170;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.918550;0.888420;,
  0.917420;0.867760;,
  0.902810;0.867760;,
  0.903170;0.888420;,
  0.888340;0.867760;,
  0.889100;0.888420;,
  0.874970;0.867760;,
  0.874970;0.888420;,
  0.832420;0.888420;,
  0.846780;0.888420;,
  0.848660;0.867760;,
  0.834600;0.867760;,
  0.860850;0.888420;,
  0.861610;0.867760;,
  0.518110;0.196370;,
  0.984330;0.196370;,
  0.984330;0.791640;,
  0.518110;0.791640;,
  0.518110;0.196370;,
  0.518110;0.791640;,
  0.984330;0.791640;,
  0.984330;0.196370;,
  -0.030101;-0.039021;,
  0.144328;0.846920;,
  -0.034911;0.723573;,
  1.031786;-0.039021;,
  1.036596;0.723573;,
  0.857357;0.846920;,
  0.500843;0.918988;,
  0.320597;0.888497;,
  0.681088;0.888497;,
  -0.034911;0.723573;,
  -0.022703;0.352031;,
  0.004205;-0.039021;,
  -0.030101;-0.039021;,
  1.036596;0.723573;,
  1.031786;-0.039021;,
  0.997480;-0.039021;,
  1.024389;0.352031;;
 }
}

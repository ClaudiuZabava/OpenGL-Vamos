// Shader-ul de fragment / Fragment shader  
 #version 330

in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;

uniform sampler2D myTexture;
uniform int VboChanger;

void main(void)
  {
	  switch (VboChanger)
	  {
		case 0: 
		  out_Color = ex_Color;
		  break;
		case 1: 
		  out_Color = mix(texture(myTexture, tex_Coord), ex_Color, 0.2);
		  break;
		default:
		 out_Color = ex_Color;
		 break;
	};
			
  }
 
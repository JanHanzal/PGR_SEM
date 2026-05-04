#version 140

const ivec2 frames = ivec2(6, 4);

smooth in vec3 position_v;    
smooth in vec2 texCoord_v;    
         
out vec4 color_f;    

uniform float time;                   
uniform sampler2D texSampler; 

const float frameDuration = 0.1;


vec4 get_Texture(int frame)
{
	int col = frame % frames.x;
	int row = frame / frames.x;

	int invertedRow = frames.y  - row;

	vec2 frame_size = vec2(1.0) / vec2(frames); 
	vec2 offset = vec2(float(col), float(invertedRow)) * frame_size;
	
	vec2 uv = (frame_size * texCoord_v) + offset;
	
	vec2 epsilon = vec2(0.005);
	uv = clamp(uv, offset + epsilon, offset + frame_size - epsilon);
		
	return texture(texSampler, uv);
}


void main()
{
	int frame = int(time / frameDuration) % (frames.x * frames.y);
	color_f = get_Texture(frame);
}
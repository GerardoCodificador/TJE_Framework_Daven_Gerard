
float sdSphere( vec3 p, float s ) {
	return length(p)-s;
}

float when_lt(float left_side, float right_side) {
	return max(sign(right_side - left_side), 0.0);
}
float when_gt(float left_side, float right_side) {
	return max(sign(left_side - right_side), 0.0);

}

uniform vec3 u_color;
uniform vec3 u_pulse_color;
uniform float u_pulse_width;
uniform vec3 u_pulse_center;
uniform float u_pulse_radius;
uniform int u_pulse_active;

uniform mat4 u_inv_vp_mat;
varying vec3 v_world_position;
void main()
{

	vec3 world_position = v_world_position;
	float mix_ratio = 0.0;
	
	if(u_pulse_active==1){
		vec3 adjusted_position = world_position-u_pulse_center;
		float dist = sdSphere(adjusted_position, u_pulse_radius);
		
		float check = when_lt(dist, 0.0) * when_gt(dist, -u_pulse_width);
		float percentage = abs(dist) / abs(u_pulse_width);
		mix_ratio = 1.0 * check - percentage;
		mix_ratio = clamp(mix_ratio, 0.0, 1.0);
	}
	if(u_pulse_active==1)
			gl_FragColor=vec4(mix(u_color,u_pulse_color,mix_ratio),1.0);
		
	else gl_FragColor=vec4(u_color,1.0);

}
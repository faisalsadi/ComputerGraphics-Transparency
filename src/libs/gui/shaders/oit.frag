#version 430

layout (early_fragment_tests) in;

#define MAX_FRAGMENTS 75
#include "compute_shading.frag"
#include "compute_shadow_factor.frag"

//in vec3 FragPos;
//in vec3 Normal;

//uniform vec4 LightPosition;
//uniform vec3 LightIntensity;
uniform vec3 objectColor;

//uniform vec4 Kd;            // Diffuse reflectivity
//uniform vec4 Ka;            // Ambient reflectivity

struct NodeType {
  vec4 color;
  float depth;
  uint next;
};

layout( binding = 0, r32ui) uniform uimage2D headPointers;
layout( binding = 0, offset = 0) uniform atomic_uint nextNodeCounter;

layout( binding = 0, std430 ) buffer linkedLists {
  NodeType nodes[];
};

uniform uint MaxNodes;

out vec4 FragColor;

subroutine void RenderPassType();

subroutine uniform RenderPassType RenderPass;



  subroutine(RenderPassType)
void pass1()
{
  // Get the index of the next empty slot in the buffer
  uint nodeIdx = atomicCounterIncrement(nextNodeCounter);

  // Is our buffer full?  If so, we don't add the fragment
  // to the list.
  if( nodeIdx < MaxNodes ) {

    // Our fragment will be the new head of the linked list, so
    // replace the value at gl_FragCoord.xy with our new node's
    // index.  We use imageAtomicExchange to make sure that this
    // is an atomic operation.  The return value is the old head
    // of the list (the previous value), which will become the
    // next element in the list once our node is inserted.
    uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);

    // Here we set the color and depth of this new node to the color
    // and depth of the fragment.  The next pointer, points to the
    // previous head of the list.
    nodes[nodeIdx].color = vec4(0,1,1,0.2);
    nodes[nodeIdx].depth = gl_FragCoord.z;
    nodes[nodeIdx].next = prevHead;
  }
 // FragColor = nodes[nodeIdx].color;

}


 subroutine(RenderPassType)
void pass2()
{
  NodeType frags[MAX_FRAGMENTS];

  int count = 0;

  // Get the index of the head of the list
  uint n = imageLoad(headPointers, ivec2(gl_FragCoord.xy)).r;

  // Copy the linked list for this fragment into an array
  while( n != 0xffffffff && count < MAX_FRAGMENTS) {
    frags[count] = nodes[n];
    n = frags[count].next;
    count++;
  }

    //merge sort
    int i, j1, j2, k;
    int a, b, c;
    int step = 1;
    NodeType leftArray[MAX_FRAGMENTS/2]; //for merge sort
    
    while (step <= count)
    {
        i = 0;
        while (i < count - step)
        {
            ////////////////////////////////////////////////////////////////////////
            //merge(step, i, i + step, min(i + step + step, count));
            a = i;
            b = i + step;
            c = (i + step + step) >= count ? count : (i + step + step);

            for (k = 0; k < step; k++)
                leftArray[k] = frags[a + k];
            
            j1 = 0;
            j2 = 0;
            for (k = a; k < c; k++)
            {
                if (b + j1 >= c || (j2 < step && leftArray[j2].depth > frags[b + j1].depth))
                    frags[k] = leftArray[j2++];
                else
                    frags[k] = frags[b + j1++];
            }
            ////////////////////////////////////////////////////////////////////////
            i += 2 * step;
        }
        step *= 2;
    } 

   
  // Traverse the array, and combine the colors using the alpha
  // channel.
  vec4 color = vec4(objectColor,0.5);
  for( int i = 0; i < count; i++ )
  {
    color = mix( color, frags[i].color, frags[i].color.a);
  }

  // Output the final color

  FragColor = color;
  //FragColor= vec4(objectColor,0.5);
 // FragColor= vec4(frags[i].color,frags[i].color.a);
 // FragColor = vec4(computeDiffuseComponent(),0.5);
 
}

void main() {
// FragColor = vec4(objectColor,0.5);
   // FragColor=vec4(1,0,0,1);
  // FragColor=objectColor;
       RenderPass();
     // FragColor = vec4(computeDiffuseComponent(),0.5);
}
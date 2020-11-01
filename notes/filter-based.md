#### Laplacian smoothing

- Diffusion flow
- Diffusion equation:

$$
\frac{\partial f(x, t)}{\partial t} = \lambda \Delta f(x, t)
$$





1.  A second-order liner partial differential equation
2. Smooth an arbitrary function $f$ on an a manifold surface by using laplace-beltrami operator
3. Discrete the equation both in space and time

- Discrete Spatial

  - $f(t) = (f(v_1, t), ..., f(v_n, t))^T$
  - Discrete Laplace-Beltrami using either the uniform or cotangent formula
  - The evolution of the function value of each vertex

  $$
  \frac{\partial f(x, t)}{\partial t} = \lambda \Delta f(x, t) = \lambda \cdot Lf(t)
  $$

  

- Temporal discretization
  - Uniform sampling : $(t, t + h, t + 2h, ..., )$
  - Explicit Euler integration:
    - $f(t + h) = f(t) + h\frac{\partial f(t)}{\partial t} = f(t) + h\lambda \cdot Lf(t)$
    - Numerically stability : a sufficiently small time step $h$
  - Implicit Euler integration:
    - $f(t + h) = f(t) + h\lambda \cdot Lf(t  + h) $
    - $(I - h\lambda \cdot L)f(t + h) = f(t)$



#### Gaussian Filter and Bilateral Filter

- Image Denoising

  - $I(p) = \frac{1}{K_p} \sum_{q \in \Omega(p)} W_s(||p - q||) I(q)$
    - $\Omega(p)$ : neighborhood of $p$
    - $W_s$: position similarity between $p$ and $q$, Gaussian function with standard deviations $\delta_s$
    - $K_p$: is the normalization term , the summation of weights
  - edge is not preserved
  - So, use bilateral filter : $I(p) = \frac{1}{K_p} \sum_{q \in \Omega(p)} W_s(||p - q||) W_r(||I(p) - I(q)||) I(q)$
    - $W_r$:  color similarity between $p$ and $q$

- Mesh case , Paper ： Bilateral Mesh Denoising

  - Vertex positions cannot simply be considered as the signal to be processed
  - Filtering a mesh using local neighborhoods
  - Main idea: local parameter space for every space vertex using the tangent plane
    - The heights of vertices over the tangent plane => gray-level values of an image
  - Update $v$:
    - $v^{new} = v + d \cdot n$

- Paper : Bilateral Normal Filtering for Mesh Denoising

  - The normal on facets are well-defined

  - Consider normal as a surface signal defined over the original mesh

  - A novel bilateral normal filter that depends on both spatial distance and signal distance

  - Recover vertex positions in global and non-iterative manner

  - $n(f_i) = \frac{1}{K_p}\sum_{f_j \in \Omega(f_i)} A_j W_s(||c_i - c_j||) W_r(||n(f_i) - n(f_j)||) \cdot n(f_j)$

  - Given the normal on each facet, determine the vertex positions to match the normal as much as possible

  - Local and Iterative Scheme

    - update the normal field

      - Normal Updating : $n_i^{t + 1} = \frac{1}{K_p} \sum_{f_j \in \Omega(f_i)} A_jW_sW_r \cdot n_j^{t}$

     

    ​      

    - update the vertex position

        - Vertex Updating : 

        - $$
        \begin{cases}
        n_f^T \cdot (x_j - x_i) = 0 \\
        n_f^T \cdot (x_k - x_j) = 0 \\
        n_f^T \cdot (x_i - x_k) = 0
        \end{cases}
        $$

        - Energy : $E = \sum_{f_k}\sum_{i, j \in f_k} (n^T_k \cdot (x_j - x_i))^2$
      
        - Linear System Solve
      
          - $x_i^{new} = x_i + \frac{1}{N_i} \sum_{f_j \in \Omega(i)} n_j \cdot (n_j^T \cdot (c_j - x_i))$
  
  - Global and Non-Iterative Scheme
  
    - Energy minimization




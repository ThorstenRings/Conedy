.. _addingNewNodes :

Node Types
//////////

Conedy provides nodes with dynamical maps, ordinary differential equations and stochastic differential equations. In addition pulse coupled oscillators and pulse coupled excitatory integrate-and-fire neurons can be integrated by an event based approach. All these integration schemes have a common interface in Conedy and can be employed in the same fashion.

See :ref:`nodes` for a full list of available node dynamics.

All node dynamics are described in ini-like reference files, which are explained in the following. If the dynamics you want to investigate is not available in Conedy, you still may implement it yourself by creating such a file, which has to be put into a directory which is specified in the config.h file. Conedy needs to be recompiled afterwards, which will be automatically issued on the next import to python if a file in this directory is modified or added.

In the following the syntax for such a file will be explained using the Rössler oscillator as an example (available as :ref:`roessler` in Conedy). Before delving into the details we give the file’s full content for the sake of an overview:

.. code-block:: c++

	[roessler]

	parameter = 4
	parametername1 = omega
	standardvalue1 = 0.89
	parametername2 = a
	standardvalue2 = 0.165
	parametername3 = b
	standardvalue3 = 0.2
	parametername4 = c
	standardvalue4 = 10.0

	integrator = stdOdeIntegrator

	dimension = 3
	dynamics =
		dxdt[0] = -omega() * x[1] - x[2] + couplingSum() - weightSum()*x[0];
		dxdt[1] = x[0] + a() * x[1];
		dxdt[2] = b() + x[2] * (x[0]-c());


Name
----

Begin with the name of the desired dynamics, enclosed in square brackets::

   [roessler]

The name should not match with a previously defined node and be a valid python token. For example, it should not begin with a number.


Parameters
----------
Specify the number of parameters of the node::

	parameter = 4

Specify for each parameter a name and a standard value::

	parametername1 = omega
	standardvalue1 = 0.89
	parametername2 = a
	standardvalue2 = 0.165
	parametername3 = b
	standardvalue3 = 0.2
	parametername4 = c
	standardvalue4 = 10.0


Equations
---------

The exact shape of the equations depends on the integrator that is used. The Rössler oscillator uses a standard integrator for ordinary differential equations::

	integrator = stdOdeIntegrator

Thus in the eqations the time derivatives of its dynamics have to be specified. See the :ref:`next subsection <integrators>` for a detailed description of this integrator and the others provided by Conedy.


Next, specify the number of dynamical variables of the node (e.g. the number of differential equations)::

	dimension = 3


Finally the equations can be defined:

.. code-block:: c++

	dynamics =
		dxdt[0] = -omega() * x[1] - x[2] + couplingSum() - weightSum()*x[0];
		dxdt[1] = x[0] + a() * x[1];
		dxdt[2] = b() + x[2] * (x[0]-c());

These equations are directly copied into a C++ file and may contain the following elements:

-	standard C constructs like basic mathematical operators, brackets or loops
-	functions from ``math.h``
-	the parameters of the dynamics as functions without arguments, e.g. ``omega()``
-	the dynamical variables such as ``x[0]``
-	``weightSum()`` which returns the sum over the weights of all edges, which are directed **from** the node.
-	``couplingSum()`` which returns the sum over the states offered by the edges, which are directed **from** the node (usually [weight of the edge]×[state of the target node])
-	The macro ``FOREACHCONNECTEDNODE`` which provides a loop over all edges directed **from** the node. In such a loop ``weight`` returns the weight of the respective edge and ``state`` returs the state of the target node. For example the first equation of the above Rössler oscillator might as well have been replaced by:

	.. code-block:: c++

		FOREACHCONNECTEDNODE(
			dxdt[0] = -omega() * x[1] - x[2] + weight*state - weight*x[0];
		)

	For an example, which requires the use of ``FOREACHCONNECTEDNODE``, see the :ref:`Kuramoto oscillator <kuramoto>`.

The counterintuitive effect of directionality is due to the internal realization of coupling. Note, that weight directions work “correctly” for :ref:`pulse-coupled oscillators <pulse-coupled>`.

.. _integrators :

Integrators
-----------

Conedy offers the following integration schemes:

mapNode
+++++++

Iteration of a dynamical map. The ``dynamics`` field should define the next state ``xprime`` directly as function of the current state ``x``.

Example:

.. code-block:: c++

	dynamics =
	xprime[0] = r() * x[0] * (-x[0] + 1)

stdOdeIntegrator
++++++++++++++++

Integration with the classical 4\ :sup:`th` order Runge Kutta method. The ``dynamics`` field should define the derivative ``dxdt`` as function of the current state ``x``.

Example:

.. code-block:: c++

	dynamics =
	dxdt[0] = -omega() * x[1] - x[2] + getCouplingSum() - weightSum()*x[0];
	dxdt[1] = x[0] + a() * x[1];
	dxdt[2] = b() + x[2]*( x[0] - c() );

gslOdeIntegrator
++++++++++++++++

Same as the ``stdOdeIntegrator``, only that numerical integration algorithms provided by the GNU Scientific Library (GSL) are used instead of Conedy’s directly implemented integrator. At the moment only those algorithms are supported, which make no use of the Jacobian. At run-time a specific stepping function can be choosen by setting "gslOdeNode_stepType" to one of the following values:

- ``gsl_odeiv_step_rk2``
- ``gsl_odeiv_step_rk4``
- ``gsl_odeiv_step_rkf45``
- ``gsl_odeiv_step_rkck``
- ``gsl_odeiv_step_rk8pd``
- ``gsl_odeiv_step_rk2imp``
- ``gsl_odeiv_step_gear1``
- ``gsl_odeiv_step_gear2``

Example::

	ns.set("gslOdeNode_stepType", "gsl_odeiv_step_rkf45")

See the `the GSL’s documentation`_ for specific information.

.. _the GSL’s documentation: http://www.gnu.org/software/gsl/manual/html_node/Ordinary-Differential-Equations.html

stdSdeIntegrator
++++++++++++++++

Explicit strong order 1.5 Taylor scheme for stochastic differential eqations.
The ``dynamics`` field should define ``dxdt`` for the deterministic part and ``dxdW`` for the stochastic part.

Example (with ``drift`` and ``diffusion`` being parameters):

.. code-block:: c++

	dynamics =
	dxdt[0] = -drift()*x[0] + couplingSum();
	dxdW[0] = diffusion();

.. _pulse-coupled:

pcoBase
+++++++
Pulse-coupled oscillators, whose state is completely defined by a phase :math:`\phi \in [0,1]`, that has a linear time evolution :math:`\frac{d\phi}{dt} = 1`. Whenever the phase of an oscillators reaches 1, the oscillator `fires`, i.e. its phase is reset to 0 and the phase of every oscillator to which an edge is directed from the firing oscillator is influenced. The influence on an oscillator is defined by its phase response curve :math:`\Delta(\phi)`, which may depend on its phase:

.. math::
   \phi' = \phi + \Delta(\phi)

The integration of these oscillators is handled by an event-based approach and is excact (as far as admitted by double precision). Note the direction of influence for edges is reversed compaired to the other integrators and coincides with the edge directionality. To define the node dynamics, the ``dynamics`` field should define the change ``delta`` (:math:`\Delta(\phi)`) to the phase of the current node (the phase response curve). For a short example (Mirollo-Strogatz):

.. code-block:: c++

	delta = a() +  b() * phase;

You may use the same elements as for the definition of differential equations. It is, however, unlikely, that ``weightSum()``, ``couplingSum()`` and ``FOREACHCONNECTEDNODE`` will be of use, given the inversion of directions. Additionally the current phase of the node is provided as ``phase``—changes of this variable are, however, without effect. Also the weight of the edge which mediated the pulse is given as ``coupling``.

If a pulse sets a node’s phase to a value greater than 1.0, this node also fires, but the excess phase remains. E.g., a node with a phase of 1.4 fires and its phase is set to 0.4 afterwards. If you wish the phase to be reset to 0.0 in this case, you can implement this in the ``dynamics`` field:

.. code-block:: c++

	delta =  a() +  b() * phase;

	if (delta + phase > 1)
		delta = 1 - phase;

Now, if the phase is about to be set to a value larger than 1.0, it is set to 1.0 instead.


pcoDelay
++++++++
Similar as ``pcoBase``, however each outgoing pulse is delayed by a time given by the parameter ``timeDelay``. (Each node dynamics based on ``pcoDelay`` automatically has nodeType_timeDelay  as first parameter.)


Using static edges
------------------


When considering networks with different kinds of edges connecting to a single node, the edge type has te be determined for every edge at run-time. In Conedy this is realized by virtual functions. For networks which homegeneous edge types, however, this may be unneccessarily slow. Furthermore, the v-tables of edges in this case, consume a non-negliable amount of memory.

Conedy offers the possiblility of making node types static, i.e. the kind of outgoing edges for this node type is set at compile time. For these nodes, no virtual function calls have to be made during integration. For cases with many unweighted edges, the memory consumption of static nodes on 64 bit machines is reduced by a factor of 4 (compared to virtual nodes). This is because in virtual nodes in addition to the target node number (4 byte) and the v-table (8 byte) most compiler will leave another 4 bytes unused due to alignment.

.. Conedy’s standard way of implementing edges are virtual functions,
.. which consume a certain amount of memory for the needed v-table. In addition whenever a node during numerical integration requires the state of a connected node, virtual function calls have to be made, which may be unneccessarily slow—depending on the circumstances.

To use static edges add::

   staticEdges = 1

to the reference file for your node. Additionally the edge type which should be associated with your node has to be defined by e. g.::

   staticEdgeType = weightedEdge(5.0)

Optionally you can choose a node type for target nodes, which eliminates another virtual function call::

   staticTargetNodeType = roessler

You can add outgoing edges to such a node as usual, however the result will always be the predefined static edge. Also weighted static edges are still affected by ``randomizeWeights``.

The following table shows memory usage of basic edges in Conedy on a 64 bit architecture.

==================   ======   =================
edge type            static   virtual functions
==================   ======   =================
edge                 4        16
staticWeightedEdge   4        16
weightedEdge         16       24
==================   ======   =================

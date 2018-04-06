/**
 * MleScheduler.java
 * Created on September 18, 2000, 11:10 AM
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.scheduler;

// Import standard Java packages.
import java.util.Vector;


/**
 * The <code>MleScheduler</code> class is used to schedule phases of execution
 * which might be required by a runtime engine. For example, a 3D game engine
 * might require four phases of execution:
 * <ol>
 * <li> a behavioral pass on a scene graph to update an actor's behavior
 * <li> a pre-render pass on a scene graph to calculate transformations
 * <li> a render pass on a scene graph to render a scene to an offscreen buffer
 * <li> a post-render pass to blit the offscreen buffer to a screen
 * </ol>
 * <p>
 * Each phase of the scheduler must be registered by calling
 * <code>addPhase</code> with an instance of the <code>MlePhase</code> class.
 * The <code>MlePhase</code> class is a collection of tasks which must be
 * executed during that phase.
 * The tasks are actually <code>java.lang.Thread</code> objects encapsulated by
 * the <code>MleTask</code> class.
 * </p>
 *
 * @see MlePhase
 * @see MleTask
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleScheduler extends Object
{
    // The number of phases.
    private Vector m_phases;
    // Flag indicating that it is ok to exit.
    private boolean m_exitOK;

    /**
     * Creates a new MleScheduler.
     */
    public MleScheduler()
    {
        m_phases = new Vector();
        m_exitOK = false;
    }
    
    /**
     * Creates a new MleScheduler with the specified number of phases,
     * <code>numPhases</code>
     *
     * @param numPhases An integer indicating the likely number of phases
     * this scheduler will have.
     */
    public MleScheduler(int numPhases)
    {
        m_phases = new Vector(numPhases);
        m_exitOK = false;
    }
    
    /**
     * Flags scheduler that it is Ok to exit.
     */
    public synchronized void setExitOk()
    {
        m_exitOK = true;
    }
    
    /**
     * Gets the number of registered phases for this scheduler.
     *
     * @return An integer representing the number of phases which have been
     * registered with this scheduler.
     */
    public synchronized int getNumberOfPhases()
    {
        return m_phases.size();
    }
    
    /**
     * Adds a phase to this scheduler.
     *
     * @param phase An instance of <code>MlePhase</code>.
     *
     * @return A boolean value is returned indicating whether the phase was
     * successfully added. If the phase was successfully added, then
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MlePhase
     */
    public synchronized boolean addPhase(MlePhase phase)
    {
        m_phases.addElement(phase);
        return true;
    }
    
    /**
     * Removes a phase from this scheduler.
     *
     * @param phase An instance of <code>MlePhase</code>.
     *
     * @return A boolean value is returned indicating whether the phase was
     * successfully deleted. If the phase was successfully deleted, then
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MlePhase
     */
    public synchronized boolean deletePhase(MlePhase phase)
    {
        return m_phases.removeElement(phase);
    }
    
    /**
     * Gets the phase at index <code>n</code>.
     *
     * @param n An integer identifying the nth phase in this scheduler.
     *
     * @return If a phase is found at the specifed location, <code>n</code>,
     * then an object of type <code>MlePhase</code> will be returned.
     * Otherwise, <b>null</b> will be returned.
     *
     * @see MlePhase
     */
    public synchronized MlePhase getPhase(int n)
    {
        MlePhase phase;
        try
		{
            phase = (MlePhase) m_phases.elementAt(n);
        } catch (IndexOutOfBoundsException e)
		{
            phase = null;
        }
        return phase;
    }
    
    /**
     * Gets the phase with the specified <code>name</code>
     *
     * @param name The name of the phase to retireve from this
     * scheduler. It is an instance of <code>java.lang.String</code>
     *
     * @return If a phase is found with the specifed <code>name</code>,
     * then an object of type <code>MlePhase</code> will be returned.
     * Otherwise, <b>null</b> will be returned.
     *
     * @see MlePhase
     */
    public synchronized MlePhase getPhase(String name)
    {
        MlePhase phase = null;
        for (int i = 0; i < m_phases.size(); i++)
        {
            MlePhase curPhase = (MlePhase) m_phases.elementAt(i);
            String curName = curPhase.getName();
            if (name.compareTo(curName) == 0)
            {
                /** Names are equal */
                phase = curPhase;
                break;
            }
        }
        return phase;
    }
    
    /**
     * Adds a task to the specified phase. The phase must have been previously
     * registered with this scheduler.
     *
     * @param phase The phase which the <code>task</code> will be added to.
     * @param task The task which will be added to the specifed
     * <code>phase</code>.
     *
     * @return A boolean value is returned indicating whether the
     * <code>task</code> was successfully added to the specifed
     * <code>phase</code>. If the task was successfully added, then
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MlePhase
     * @see MleTask
     */
    public synchronized boolean addTask(MlePhase phase, MleTask task)
    {
        if (m_phases.contains(phase))
        {
            return phase.addTask(task);
        }
        return false;
    }
    
    /**
     * Removes the a task from the specified phase. The phase must be
     * registered with this scheduler.
     *
     * @param phase The phase which the <code>task</code> will be removed from.
     * @param task The task which will be removed from the specifed
     * <code>phase</code>.
     *
     * @return A boolean value is returned indicating whether the
     * <code>task</code> was successfully deleted from the specifed
     * <code>phase</code>. If the task was successfully deleted, then
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MlePhase
     * @see MleTask
     */
    public synchronized boolean deleteTask(MlePhase phase, MleTask task)
    {
        if (m_phases.contains(phase))
        {
            return phase.deleteTask(task);
        }
        return false;
    }
    
    /**
     * Executes scheduled phases. The order of execution is based on the order
     * in which the phases were registered with this scheduler. A phase must
     * complete before the next phase is executed. A phase is complete when all
     * tasks registered with that phase have completed and are no longer
     * running.
     * <p>
     * When all phases have been processed, the execution continues again with
     * the first phase. Thus the phases are executed using a round-robin
     * algorithm.
     * </p><p>
     * To discontinue execution, the scheduler must be flagged by specifying
     * that it is Ok to exit. This is done using the <code>setExitOk</code>
     * method. The scheduler will finish executing the current phase and exit
     * prior to starting the next phase.
     * </p><p>
     * No state is maintained between phases. Therefore an application which
     * has discontinued execution by setting the exit Ok flag should not expect
     * the scheduler to start up again at the next phase if this method is
     * invoked again. <code>run</code> will always invalidate the exit Ok flag
     * and start at the first scheduled phase.
     */
    public synchronized void run()
    {
        m_exitOK = false;
        for (int i = 0; i < m_phases.size(); i++)
        {
            /* Fork off tasks in task list scheduled for this phase. */ 
            MlePhase phase = (MlePhase) m_phases.elementAt(i);

            /* Wait for phase[i] to complete. */
            phase.run();
            if (m_exitOK)
                break;
        }
    }
    
    /**
     * Run the specified phase.
     * 
     * @param phase The phase to execute.
     */
    public synchronized void go(MlePhase phase)
    {
        phase.run();
    }
    
    /**
     * Dumps the list of registered phases for this scheduler. It will also
     * list the tasks associated with each phase.
     */
    public synchronized void dump()
    {
        for (int i = 0; i < this.getNumberOfPhases(); i++)
        {
            MlePhase phase = (MlePhase) this.getPhase(i);
            
            System.out.print("Phase ");
            System.out.print(i+1);
            System.out.print(": ");
            System.out.println(phase.getName());
            
            for (int j = 0; j < phase.getNumberOfTasks(); j++)
            {
                MleTask task = phase.getTask(j);
                
                System.out.print("\tTask ");
                System.out.print(j+1);
                System.out.print(": ");
                if (task.getName() != null)
                    System.out.println(task.getName());
                else
                    System.out.println("null");
            }
        }
    }
    
}

/**
 * MleTask.java
 * Created on September 18, 2000, 4:17 PM
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

/**
 * <code>MleTask</code> is a class that manages a named thread.
 * <p>
 * MleTask encapsulates an autonamous action, using the
 * <code>java.lang.Thread</code> class, for the Magic Lantern scheduler,
 * <code>MleScheduler</code>.
 * </p>
 *
 * @see MlePhase
 * @see MleScheduler
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleTask extends Object
{
    // The runnable object.
    private Runnable m_task;
    // A thread that executes the Runnable.
    private Thread m_thread;
    // The name of the task.
    private String m_name = null;
    // A flag indicating whether the task is running.
    private boolean m_running;

    // Hide the default consructor.
    private MleTask()
    {}

    /**
     * Creates a new MleTask with a reference
     * to the specified Runnable, <i>task</i>.
     *
     * @param task An instance of a Runnable object.
     *
     * @see java.lang.Runnable
     */
    public MleTask(Runnable task)
    {
        m_task = task;
        m_running = false;
    }

    /**
     * Creates a new MleTask with the specified <i>name</i> and a reference
     * to the specified Runnable, <i>task</i>.
     *
     * @param task An instance of a Runnable object.
     * @param name The name of the task; an instance of String.
     *
     * @see java.lang.Runnable
     * @see java.lang.String
     */
    public MleTask(Runnable task, String name)
    {
        m_task = task;
        m_name = name;
        m_running = false;
    }

    /**
     * Retrieves the name of this task.
     *
     * @return The name of the task as a String.
     *
     * @see java.lang.String
     */
    public String getName()
    {
        return m_name;
    }
    
    /**
     * Executes task by starting a thread with the Runnable
     * specified during construction.
     */
    public void invoke()
    {
        m_running = true;
        if (m_name != null)
            m_thread = new Thread(m_task,m_name);
        else
            m_thread = new Thread(m_task);
        m_thread.start();
    }
    
    /**
     * Checks if this task is still running.
     *
     * @return A boolean indicating whether the task is still running.
     * If the task is running, <b>true</b> is returned;
     * otherwise, <b>false</b> will be returned.
     */
    public boolean isRunning()
    {
        boolean status;
    
        if (m_running == false)
        {
            status = false;
        } else
        {
            if (m_thread.isAlive())
            {
                status = true;
            } else
            {
                m_running = false;
                status = false;
            }
        }
        return status;
    }
    
}

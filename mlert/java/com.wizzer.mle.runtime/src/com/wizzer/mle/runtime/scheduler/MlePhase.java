/**
 * MlePhase.java
 * Created on September 18, 2000, 2:00 PM
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
import java.util.*;

/**
 * The <code>MlePhase</code> class is used to manage a set of tasks. Each task
 * is represented by an instance of the <code>MleTask</code> class.
 * For convenience the <code>MlePhase</code> may be identified with a
 * <code>String</code>; however, by default the <code>MlePhase</code> is
 * unnamed.
 * <p>
 * This class is used to register a task or group of tasks which must be
 * executed in context of a scheduled phase. In addition to providing methods
 * for adding and deleting a task, the <code>MlePhase</code> class provides a
 * mechanism for running all of the associated tasks.
 * When the tasks are executed, they will be invoked asynchronously.
 * However, <code>MlePhase</code> will not return until all of the tasks have
 * been completed.
 * </p>
 *
 * @see MleTask
 * @see MleScheduler
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MlePhase extends Object
{
    // The list of tasks.
    private Vector m_tasks;
    // The name of the phase.
    private String m_name;

    /**
     * Creates a new MlePhase.
     */
    public MlePhase()
    {
        m_name = null;
        m_tasks = new Vector();
    }
    
    /**
     * Creates a new MlePhase with the specified <i>name</i>.
     *
     * @param name A <code>java.lang.String</code> identifying the name
     * of the phase.
     *
     * @see java.lang.String
     */
    public MlePhase(String name)
    {
        m_name = name;
        m_tasks = new Vector();
    }
    
    /**
     * Creates a new MlePhase with the specified number of tasks,
     * <code>numTasks</code>. The phase will be unnamed.
     *
     * @param numTasks An integer indicating the number of tasks
     * this phase will have.
     */
    public MlePhase(int numTasks)
    {
        m_name = null;
        m_tasks = new Vector(numTasks);
    }
    
    /**
     * Creates a new MlePhase with the specified <code>name</code> and
     * number of tasks, <code>numTasks</code>.
     *
     * @param name A <code>java.lang.String</code> identifying the name
     * of the phase.
     * @param numTasks An integer indicating the number of tasks
     * this phase will have.
     *
     * @see java.lang.String
     */
    public MlePhase(String name,int numTasks)
    {
        m_name = name;
        m_tasks = new Vector(numTasks);
    }
    
    /**
     * Sets the name of this phase.
     *
     * @param name A <code>java.lang.String</code> identifying the name
     * of this phase.
     *
     * @see java.lang.String
     */
    public synchronized void setName(String name)
    {
        m_name = name;
    }
    
    /**
     * Gets the name of this phase.
     *
     * @return A <code>java.lang.String</code> identifying the name
     * of this phase.
     */
    public synchronized String getName()
    {
        return m_name;
    }
    
    /**
     * Gets the number of registered tasks for this phase.
     *
     * @return An integer representing the number of tasks belonging
     * to this phase.
     */
    public synchronized int getNumberOfTasks()
    {
        return m_tasks.size();
    }
    
    /**
     * Adds a task to this phase.
     *
     * @param task An instance of the MleTask class.
     *
     * @return A boolean indicating whether the task was successfully added.
     * If the task was added successfully,
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MleTask
     */
    public synchronized boolean addTask(MleTask task)
    {
        m_tasks.addElement(task);
        return true;
    }
    
    /**
     * Removes a task from this phase.
     *
     * @param task An instance of the MleTask class.
     *
     * @return A boolean indicating whether the task was successfully deleted.
     * If the task was successfully removed,
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MleTask 
     */
    public synchronized boolean deleteTask(MleTask task)
    {
        return m_tasks.removeElement(task);
    }
    
    /**
     * Removes the nth task from this phase.
     *
     * @param n An integer identifying the nth task in this phase.
     *
     * @return A boolean indicating whether the task was successfully deleted.
     * If the task was successfully removed,
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see MleTask
     */
    public synchronized boolean deleteTask(int n)
    {
        MleTask task = this.getTask(n);
        
        if (task != null)
        {
            return deleteTask(task);
        } else
        {
            return false;
        }
    }
    
    /**
     * Removes the task with the specified name from this phase.
     *
     * @param name A <code>java.lang.String</code> identifying the name
     * of the task to be removed.
     *
     * @return A boolean indicating whether the task was successfully deleted.
     * If the task was successfully removed,
     * <b>true</b> is returned; otherwise, <b>false</b> will be returned.
     *
     * @see java.lang.String
     */
    public synchronized boolean deleteTask(String name)
    {
        MleTask task = this.getTask(name);
        
        if (task != null)
        {
            return deleteTask(task);
        } else
        {
            return false;
        }
    }
    
    /**
     * Retrieves the task at index n. The task is not removed from this phase.
     *
     * @param n An integer identifying the nth task in this phase.
     *
     * @return If a task is found at the specifed location, <code>n</code>,
     * then an object of type <code>MleTask</code> will be returned.
     * Otherwise, <b>null</b> will be returned.
     *
     * @see MleTask
     */
    public synchronized MleTask getTask(int n)
    {
        MleTask task;
        try
		{
            task = (MleTask) m_tasks.elementAt(n);
        } catch (IndexOutOfBoundsException e)
		{
            task = null;
        }
        return task;
    }
    
    /**
     * Retrieves the task with the specified <i>name</i>.
     *
     * @param name A <i>java.lang.String</i> identifying the name
     * of the task to be retrieved.
     *
     * @return If a task is found matching the specified <i>name</i>
     * then an object of type <i>MleTask</i> will be returned.
     * Otherwise, <b>null</b> will be returned.
     *
     * @see MleTask
     * @see java.lang.String
     */
    public synchronized MleTask getTask(String name)
    {
        MleTask task = null;
        for (int i = 0; i < m_tasks.size(); i++)
        {
            MleTask curTask = (MleTask) m_tasks.elementAt(i);
            String curName = curTask.getName();
            if (name.compareTo(curName) == 0)
            {
                /* Names are equal */
                task = curTask;
                break;
            }
        }
        return task;
    }
    
    /**
     * Executes the tasks registered with this phase. <code>run</code>
     * will not return unitl all tasks have been completed.
     */
    public synchronized void run()
    {
        /*
        System.out.print("*** EXECTUING PHASE ");
        System.out.print(m_name);
        System.out.println(" ***");
        System.out.flush();
        */
        
        /* Invoke tasks which have been registered. */
        for (int i = 0; i < m_tasks.size(); i++)
        {
            MleTask task = (MleTask) m_tasks.elementAt(i);
            task.invoke();
        }
        
        /* Wait for all tasks to complete before returning */
        boolean tasksCompleted = false;
        waitForTaskCompletion:
        while (! tasksCompleted)
        {
            for (int i = 0; i < m_tasks.size(); i++)
            {
                MleTask task = (MleTask) m_tasks.elementAt(i);
                if (task.isRunning())
                    continue waitForTaskCompletion;
            }
            tasksCompleted = true;
        }
    }
    
}

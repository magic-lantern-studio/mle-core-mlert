// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.event;

/**
 * This class is a container for "cued" events.
 * <p>
 * Using the theater paradigm, a cue is the words or action at which an actor is expected
 * to deliver a line or a crew member is expected to perform a task. Thus, in
 * Magic Lantern it can be considered a named trigger for a particular event.
 * </p>
 * 
 * @author Mark S. Millard
 */
public class MleCue
{
	/** The cue's name. */
    protected String m_name = null;
    /** The associated <code>MleEvent</code> identifier. */
    protected int m_id = MleEvent.MLE_EVENT_INVALID_ID;
    /** Data associated with the cue. */
    protected Object m_data = null;
    
    // Hide the default constructor.
    private MleCue() {}
    
    /**
     * Constructs a "Cue", associating the logical name with a Magic Lantern
     * event identifier.
     * 
     * @param name The name of the <code>Cue</code>.
     * @param id The associated event identifier.
     */
    public MleCue(String name, int id)
    {
    	super();
    	
    	m_name = new String(name);
    	m_id = id;
    }
    
    /**
     * A copy constructor.
     * 
     * @param cue The <b>Cue</b> to duplicate.
     */
    public MleCue(MleCue cue)
    {
    	super();
    	
    	m_name = new String(cue.getName());
    	m_id = cue.getId();
    	m_data = cue.getData();
    }
    
    /**
     * Get the name of the <code>Cue</code>.
     * 
     * @return The name is returned as a <code>String</code>
     */
    public String getName()
    {
    	return m_name;
    }
    
    /**
     * Get the event identifier for the <code>Cue</code>.
     * 
     * @return The identifier is returned as an integer.
     */
    public int getId()
    {
    	return m_id;
    }
    
    /**
     * Get the data associated with the <code>Cue</code>.
     * 
     * @return A generic <code>Object</code> is returned. <b>null</b> may be returned
     * if no data has been set for this <code>Cue</code>.
     */
    public Object getData()
    {
    	return m_data;
    }
    
    /**
     * Set the data to be associated with the <code>Cue</code>.
     * 
     * @param data An <code>Object</code> encapsulating the associated data.
     * This may be <b>null</b>.
     */
    public void setData(Object data)
    {
    	m_data = data;
    }
    
    @Override
    public boolean equals(Object obj)
    {
    	boolean retValue = false;
    	
    	if (obj instanceof MleCue)
    	{
    		MleCue cue = (MleCue) obj;
    		if (m_name.equals(cue.getName()) && (m_id == cue.getId()))
    		{
    			if ((m_data == null) && (cue.m_data == null))
    				retValue = true;
    			else if ((m_data == null) && (cue.m_data != null))
    				retValue = false;
    			else if ((m_data != null) && (cue.m_data == null))
    				retValue = false;
    			else if (m_data.equals(cue.getData()))
    		        retValue = true;
    		}
    	}

    	return retValue;
    }
}

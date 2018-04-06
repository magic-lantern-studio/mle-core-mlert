// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.event;

// Import Magic Lantern classes.

/**
 * A property change event.
 * <p>
 * This class is used by <code>IMleObject</code>s (i.e. <code>Actor</code>s and
 * <code>Set</code>s) to notify <code>IMlePropChangeListeners</code> of changes
 * made to their properties.
 * </p>
 * 
 * @author Mark S. Millard
 */
public class MlePropChangeEvent extends MleEvent
{
	// A unique serialization identifier.
	private static final long serialVersionUID = -8860595120195784476L;

	/** The source that caused this event to be fired. */
	public Object m_source;
	
	/** The name of the property. */
	protected String m_name;
	/** The old value of the property. */
	protected Object m_oldValue;
	/** The new value of the property. */
	protected Object m_newValue;
	
	/**
	 * Constructs a new <code>MlePropChangeEvent.</code>
	 * 
	 * @param source The object that caused this event to be fired.
	 * @param propertyName the name of the property that has changed.
	 * @param oldValue The old value of the property.
	 * @param newValue The new value of the property.
	 */
    public MlePropChangeEvent(Object source, String propertyName, Object oldValue, Object newValue)
    {
    	super(source, 0);
 
    	m_source = source;
    	m_name = propertyName;
    	m_oldValue = oldValue;
    	m_newValue = newValue;
    }
    
    /**
     * Gets the name of the property that was changed.
     * 
     * @return The name of the property that was changed is returned.
     */
    public String getPropertyName()
    {
    	return m_name;
    }
    
    /**
     * Gets the old value for the property, expressed as an <code>Object</code>.
     * 
     * @return The old value for the property, expressed as an <code>Object</code>
     * is returned. May be <b>null</b> if multiple values have changed, as in a 
     * property array.
     */
    public Object getOldValue()
    {
    	return m_oldValue;
    }
    
    /**
     * Gets the new value for the property, expressed as an <code>Object</code>.
     * 
     * @return The new value for the property, expressed as an <code>Object</code>
     * is returned. May be <b>null</b> if multiple values have changed, as in a 
     * property array.
     */
    public Object getNewValue()
    {
    	return m_newValue;
    }
}

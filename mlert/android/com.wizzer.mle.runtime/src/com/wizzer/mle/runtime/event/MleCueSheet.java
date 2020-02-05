// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.event;

// Import standard Java class.
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.core.MleRuntimeException;

/**
 * This class manages a collection of cues.
 * 
 * @author Mark S. Millard
 */
public class MleCueSheet
{
	/** The collection of cues. */
    protected HashMap<String, MleCue> m_cues = null;
    
    /**
     * The default constructor.
     */
    public MleCueSheet()
    {
    	m_cues = new HashMap<String, MleCue>();
    }
    
    /**
     * A copy constructor.
     * 
     * @param sheet The <code>CueSheet</code> to duplicate.
     */
    public MleCueSheet(MleCueSheet sheet)
    {
    	m_cues = new HashMap<String, MleCue>();
    	
    	Collection<MleCue> cues = sheet.getCues();
    	Iterator<MleCue> iter = cues.iterator();
    	while (iter.hasNext())
    	{
    		MleCue next = iter.next();
    		MleCue copy = new MleCue(next);
    		m_cues.put(copy.getName(), copy);
    	}
    }
    
    /**
     * Retrieve the named <i>Cue</i>.
     * 
     * @param name The name of the <i>Cue</i>.
     * 
     * @return A <code>MleCue</code> is returned.
     */
    public MleCue getCue(String name)
    {
    	return m_cues.get(name);
    }
    
    /**
     * Add the <i>Cue</i> to the sheet.
     * <p>
     * If the sheet previously contained a mapping for this cue, the old value is replaced.
     * </p>
     * 
     * @param cue The cue to add.
     * 
     * @return The previous cue associated with specified name will be returned.
     * <b>null</b> will be returned if there was no mapping for the <code>cue</code>'s
     * name.
     * 
     * @throws MleRuntimeException This exception is thrown if the input argument
     * is <b>null</b>, the <code>MleCue</code>'s name is <b>null</b>, or the
     * <code>MleCue</code>'s id is invalid.
     */
    public MleCue addCue(MleCue cue) throws MleRuntimeException
    {
    	if (cue == null)
    		throw new MleRuntimeException("Cue argument must not be null.");
    	if (cue.getName() == null)
    		throw new MleRuntimeException("Cue's name must not be null.");
    	if (cue.getId() == MleEvent.MLE_EVENT_INVALID_ID)
    		throw new MleRuntimeException("Cue's id is invalid.");
    	
    	MleCue oldValue = (MleCue) m_cues.put(cue.getName(), cue);
    	return oldValue;
    }
    
    /**
     * Remove the named <i>Cue</i> from the sheet.
     * 
     * @param name The name of the <i>Cue</i> to remove.
     * 
     * @return The previous cue associated with specified name will be returned.
     * <b>null</b> will be returned if there was no mapping for the <code>cue</code>'s
     * name.
     * 
     * @throws MleRuntimeException This exception is thrown if the input argument
     * is <b>null</b>.

     */
    public MleCue removeCue(String name) throws MleRuntimeException
    {
    	if (name == null)
    		throw new MleRuntimeException("Cue argument must not be null.");

    	MleCue oldValue = m_cues.remove(name);
    	return oldValue;
    }
    
    /**
     * Remove all cues from the sheet.
     */
    public void clear()
    {
    	m_cues.clear();
    }
    
    /**
     * Get the number of cues in the sheet.
     * 
     * @return The number of cues in the sheet is returned.
     */
    public int size()
    {
    	return m_cues.size();
    }
    
    /**
     * Returns a collection view of the cues contained in this sheet.
     * The collection is backed by the sheet, so changes to the sheet
     * are reflected in the collection, and vice-versa. The collection
     * supports element removal, which removes the corresponding mapping from this sheet.
     * 
     * @return A collection view of the cues contained in this sheet is returned.
     */
    public Collection<MleCue> getCues()
    {
    	Collection<MleCue> cues = m_cues.values();
    	return cues;
    }
    
    @Override
    public boolean equals(Object obj)
    {
    	boolean retValue = false;
    	
    	if (obj instanceof MleCueSheet)
    	{
    		MleCueSheet testCues = (MleCueSheet) obj;
    		
    		if (m_cues.equals(testCues.m_cues))
    			retValue = true;
    	}
    	
    	return retValue;
    }
}

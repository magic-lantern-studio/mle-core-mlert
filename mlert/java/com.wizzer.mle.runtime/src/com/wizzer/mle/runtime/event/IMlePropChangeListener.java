// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.event;

// Import Magic Lantern classes.

/**
 * A "PropChange" event gets fired whenever an <code>Actor</code> changes a "bound" property.
 * You can register a <code>IMlePropChangeListener</code> with a source <code>Actor</code>
 * so as to be notified of any bound property updates.
 * 
 * @author Mark S. Millard
 */
public interface IMlePropChangeListener
{
	/**
	 * This method gets called when a bound property is changed. 
	 * 
	 * @param event A <code>MlePropChangeEvent</code> object describing the event
	 * source and the property that has changed.
	 */
    public void propChangedEvent(MlePropChangeEvent event);
}

// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.unittest;

// Import standard Java classes.
import java.util.Collection;
import java.util.Iterator;

//Import JUnit classes.
import junit.framework.TestCase;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.MleCue;
import com.wizzer.mle.runtime.event.MleCueSheet;

/**
 * This class is a unit test for com.wizzer.mle.runtime.event.MleCueSheet.
 * 
 * @author Mark S. Millard
 */
public class CueSheetTest extends TestCase
{
	protected void setUp() throws Exception
	{
		super.setUp();
	}

	protected void tearDown() throws Exception
	{
		super.tearDown();
	}

	public void testMleCueSheet()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
	}

	public void testMleCueSheetMleCueSheet()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue1 = new MleCue("One", 1);
		assertNotNull(cue1);
		MleCue cue2 = new MleCue("Two", 2);
		assertNotNull(cue2);
		MleCue cue3 = new MleCue("Three", 3);
		assertNotNull(cue3);
		MleCue cue4 = new MleCue("Four", 4);
		assertNotNull(cue4);
		assertEquals(cueSheet.size(), 0);
		MleCue cue5 = new MleCue("Five", 5);
		assertNotNull(cue5);
		try {
			cueSheet.addCue(cue1);
			cueSheet.addCue(cue2);
			cueSheet.addCue(cue3);
			cueSheet.addCue(cue4);
			cueSheet.addCue(cue5);
			assertEquals(cueSheet.size(), 5);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}

		MleCueSheet copy = new  MleCueSheet(cueSheet);
		assertEquals(copy.size(), 5);
		assertTrue(copy.equals(cueSheet));
	}

	public void testGetCue()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue1 = new MleCue("One", 1);
		assertNotNull(cue1);
		MleCue cue2 = new MleCue("Two", 2);
		assertNotNull(cue2);
		MleCue cue3 = new MleCue("Three", 3);
		assertNotNull(cue3);
		MleCue cue4 = new MleCue("Four", 4);
		assertNotNull(cue4);
		assertEquals(cueSheet.size(), 0);
		MleCue cue5 = new MleCue("Five", 5);
		assertNotNull(cue5);
		try {
			cueSheet.addCue(cue1);
			cueSheet.addCue(cue2);
			cueSheet.addCue(cue3);
			cueSheet.addCue(cue4);
			cueSheet.addCue(cue5);
			assertEquals(cueSheet.size(), 5);
			
			MleCue tmp = cueSheet.getCue("Three");
			assertNotNull(tmp);
			assertEquals(tmp.getName(),"Three");
			assertEquals(tmp.getId(),3);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
	}

	public void testAddCue()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue = new MleCue("One", 1);
		assertNotNull(cue);
		assertEquals(cueSheet.size(), 0);
		try {
			cueSheet.addCue(cue);
			assertEquals(cueSheet.size(), 1);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
	}

	public void testRemoveCue()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue = new MleCue("One", 1);
		assertNotNull(cue);
		assertEquals(cueSheet.size(), 0);
		try {
			cueSheet.addCue(cue);
			assertEquals(cueSheet.size(), 1);
			cueSheet.removeCue("One");
			assertEquals(cueSheet.size(), 0);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
	}

	public void testClear()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue = new MleCue("One", 1);
		assertNotNull(cue);
		assertEquals(cueSheet.size(), 0);
		try {
			cueSheet.addCue(cue);
			assertEquals(cueSheet.size(), 1);
			cueSheet.clear();
			assertEquals(cueSheet.size(), 0);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
	}

	public void testSize()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue1 = new MleCue("One", 1);
		assertNotNull(cue1);
		MleCue cue2 = new MleCue("Two", 2);
		assertNotNull(cue2);
		MleCue cue3 = new MleCue("Three", 3);
		assertNotNull(cue3);
		MleCue cue4 = new MleCue("Four", 4);
		assertNotNull(cue4);
		assertEquals(cueSheet.size(), 0);
		MleCue cue5 = new MleCue("Five", 5);
		assertNotNull(cue5);
		try {
			cueSheet.addCue(cue1);
			cueSheet.addCue(cue2);
			cueSheet.addCue(cue3);
			cueSheet.addCue(cue4);
			cueSheet.addCue(cue5);
			assertEquals(cueSheet.size(), 5);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
	}

	public void testGetCues()
	{
		MleCueSheet cueSheet = new MleCueSheet();
		assertNotNull(cueSheet);
		MleCue cue1 = new MleCue("One", 1);
		assertNotNull(cue1);
		MleCue cue2 = new MleCue("Two", 2);
		assertNotNull(cue2);
		MleCue cue3 = new MleCue("Three", 3);
		assertNotNull(cue3);
		MleCue cue4 = new MleCue("Four", 4);
		assertNotNull(cue4);
		assertEquals(cueSheet.size(), 0);
		MleCue cue5 = new MleCue("Five", 5);
		assertNotNull(cue5);
		try {
			cueSheet.addCue(cue1);
			cueSheet.addCue(cue2);
			cueSheet.addCue(cue3);
			cueSheet.addCue(cue4);
			cueSheet.addCue(cue5);
			assertEquals(cueSheet.size(), 5);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
		
		Collection<MleCue> cues = cueSheet.getCues();
		Iterator<MleCue> iter = cues.iterator();
		int count = 0;
		while (iter.hasNext())
		{
			iter.next();
			count++;
		}
		assertEquals(count, 5);
	}

	public void testEqualsObject()
	{
		MleCueSheet cueSheet1 = new MleCueSheet();
		assertNotNull(cueSheet1);
		MleCue cue1 = new MleCue("One", 1);
		assertNotNull(cue1);
		MleCue cue2 = new MleCue("Two", 2);
		assertNotNull(cue2);
		MleCue cue3 = new MleCue("Three", 3);
		assertNotNull(cue3);
		MleCue cue4 = new MleCue("Four", 4);
		assertNotNull(cue4);
		assertEquals(cueSheet1.size(), 0);
		MleCue cue5 = new MleCue("Five", 5);
		assertNotNull(cue5);
		try {
			cueSheet1.addCue(cue1);
			cueSheet1.addCue(cue2);
			cueSheet1.addCue(cue3);
			cueSheet1.addCue(cue4);
			cueSheet1.addCue(cue5);
			assertEquals(cueSheet1.size(), 5);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
		
		MleCueSheet cueSheet2 = new MleCueSheet();
		assertNotNull(cueSheet2);
		MleCue cue6 = new MleCue("One", 1);
		assertNotNull(cue6);
		MleCue cue7 = new MleCue("Two", 2);
		assertNotNull(cue7);
		MleCue cue8 = new MleCue("Three", 3);
		assertNotNull(cue8);
		MleCue cue9 = new MleCue("Four", 4);
		assertNotNull(cue9);
		assertEquals(cueSheet2.size(), 0);
		MleCue cue10 = new MleCue("Five", 5);
		assertNotNull(cue10);
		try {
			cueSheet2.addCue(cue6);
			cueSheet2.addCue(cue7);
			cueSheet2.addCue(cue8);
			cueSheet2.addCue(cue9);
			cueSheet2.addCue(cue10);
			assertEquals(cueSheet2.size(), 5);
		} catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
		
		assertTrue(cueSheet1.equals(cueSheet2));
		
		MleCue cue11 = new MleCue("Eleven", 11);
		assertNotNull(cue11);
		try {
			cueSheet2.addCue(cue11);
		}  catch (MleRuntimeException e) {
			fail("MleRuntimeException thrown: " + e.getMessage());
		}
		assertFalse(cueSheet1.equals(cueSheet2));
	}

}

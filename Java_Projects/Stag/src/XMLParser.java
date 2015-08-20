import java.io.*;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;

import java.io.File;

public class XMLParser {
	private Document document;
	private Node root;
	private NodeList rootNodes;
	
	public XMLParser(String fileName) {
		try{
			DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			document = builder.parse(new File(fileName));
			root = document.getDocumentElement();
			rootNodes = document.getChildNodes();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public Document getDocument() {
		return document;
	}
	
	public Node getRoot() {
		return root;
	}
	
	public NodeList getRootNodes() {
		return rootNodes;
	}
}
